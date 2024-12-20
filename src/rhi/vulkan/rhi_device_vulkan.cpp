#include "rhi_device_vulkan.h"

#include "SDL3/SDL_vulkan.h"

#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

struct RHICommandQueue {
	uint32_t vk_family_id;
	VkQueue vk_queue = VK_NULL_HANDLE;
};

struct RHICommandPool {
	VkCommandPool vk_command_pool;
};

struct RHICommandBuffer {
	VkCommandBuffer vk_cmd_buf;
};

struct RHISwapChain {
	VkSurfaceKHR vk_surface = VK_NULL_HANDLE;
	VkSwapchainKHR vk_swapchain = VK_NULL_HANDLE;
	std::vector<RHITexture> textures;
};

struct RHIFence {
	VkSemaphore vk_semaphore = VK_NULL_HANDLE;
};

struct RHITexture {
	VkImage vk_image = VK_NULL_HANDLE;
};

RHIDeviceVulkan::RHIDeviceVulkan(RHIContextVulkan *ctx, vkb::PhysicalDevice &physical_device) {
	context = ctx;
	// create device.
	vkb::DeviceBuilder builder(physical_device);
	auto ret = builder.build();
	if (!ret) {
		throw std::runtime_error(
				std::string("Failed to create device. Error: ") +
				ret.error().message());
	}
	device = ret.value();
	api = device.make_table();

	// graphics queue.
	auto ret_qfid = device.get_queue_index(vkb::QueueType::graphics);
	if (!ret_qfid) {
		throw std::runtime_error(ret.error().message());
	}
	queue = new RHICommandQueue{};
	queue->vk_family_id = ret_qfid.value();
	api.getDeviceQueue(ret_qfid.value(), 0, &queue->vk_queue);

	// create vma.
	VmaVulkanFunctions vma_funcs = {
		.vkGetInstanceProcAddr = context->instance.fp_vkGetInstanceProcAddr,
		.vkGetDeviceProcAddr = context->instance.fp_vkGetDeviceProcAddr
	};
	VmaAllocatorCreateInfo alloc_info = {
		.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
		.physicalDevice = physical_device,
		.device = device,
		.pVulkanFunctions = &vma_funcs,
		.instance = context->instance
	};
	VK_CHECK(vmaCreateAllocator(&alloc_info, &vma_allocator));
}

RHIDeviceVulkan::~RHIDeviceVulkan() {
	delete queue;
	vmaDestroyAllocator(vma_allocator);
	vkb::destroy_device(device);
}

RHICommandQueue *RHIDeviceVulkan::get_command_queue(RHICommandQueueType type) {
	return queue;
}

RHICommandPool *RHIDeviceVulkan::create_command_pool(RHICommandQueueType type) {
	VkCommandPoolCreateInfo ci{};
	ci.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	ci.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	ci.queueFamilyIndex = queue->vk_family_id;
	VkCommandPool pool;
	VK_CHECK(api.createCommandPool(&ci, nullptr, &pool));
	return new RHICommandPool{
		.vk_command_pool = pool
	};
}

void RHIDeviceVulkan::destroy_command_pool(RHICommandPool *pool) {
	api.destroyCommandPool(pool->vk_command_pool, nullptr);
	delete pool;
}

void RHIDeviceVulkan::reset_command_pool(RHICommandPool *pool) {
	api.resetCommandPool(pool->vk_command_pool, 0);
}

RHICommandBuffer *RHIDeviceVulkan::create_command_buffer(RHICommandPool *pool) {
	VkCommandBufferAllocateInfo ci = {};
	ci.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	ci.commandPool = pool->vk_command_pool;
	ci.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	ci.commandBufferCount = 1;
	VkCommandBuffer cmd_buff;
	VK_CHECK(api.allocateCommandBuffers(&ci, &cmd_buff));
	return new RHICommandBuffer{
		.vk_cmd_buf = cmd_buff
	};
}

void RHIDeviceVulkan::destroy_command_buffer(RHICommandPool *pool, RHICommandBuffer *cb) {
	api.freeCommandBuffers(pool->vk_command_pool, 1, &cb->vk_cmd_buf);
	delete cb;
}

void RHIDeviceVulkan::begin_command_buffer(RHICommandBuffer *cmd_buffer) {
	VkCommandBufferBeginInfo info = {
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		nullptr,
		VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
	};
	VK_CHECK(api.beginCommandBuffer(cmd_buffer->vk_cmd_buf, &info));
}

void RHIDeviceVulkan::end_command_buffer(RHICommandBuffer *cmd_buffer) {
	VK_CHECK(api.endCommandBuffer(cmd_buffer->vk_cmd_buf));
}

RHISwapChain *RHIDeviceVulkan::create_swapchain(SDL_Window *window, RHICommandQueue *queue, RHIFormat format, uint32_t count) {
	VkSurfaceKHR vk_surface;
	SDL_Vulkan_CreateSurface(window, context->instance, nullptr, &vk_surface);

	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	vkb::SwapchainBuilder builder{ device, vk_surface };
	auto ret = builder.set_desired_format(VkSurfaceFormatKHR{
												  .format = get_format(format),
												  .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR })
					   .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
					   .set_desired_extent(w, h)
					   .add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
					   .build();
	if (!ret) {
		throw std::runtime_error(
				std::string("Failed to create swapchain. Error: ") +
				ret.error().message());
	}

	auto swapchain = ret.value();
	auto images = swapchain.get_images().value();
	std::vector<RHITexture> textures;
	for (auto i : images) {
		textures.push_back({ .vk_image = i });
	}
	return new RHISwapChain{
		.vk_surface = vk_surface,
		.vk_swapchain = ret.value(),
		.textures = std::move(textures)
	};
}

void RHIDeviceVulkan::destroy_swapchain(RHISwapChain *swapchain) {
	api.destroySwapchainKHR(swapchain->vk_swapchain, nullptr);
	context->api.destroySurfaceKHR(swapchain->vk_surface, nullptr);
	delete swapchain;
}

RHIFence *RHIDeviceVulkan::create_fence(uint64_t init_value) {
	RHIFence *fence = new RHIFence{};

	VkSemaphoreTypeCreateInfo timeline_ci;
	timeline_ci.sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO;
	timeline_ci.pNext = nullptr;
	timeline_ci.semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE;
	timeline_ci.initialValue = init_value;

	VkSemaphoreCreateInfo ci;
	ci.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	ci.pNext = &timeline_ci;
	ci.flags = 0;

	VK_CHECK(api.createSemaphore(&ci, nullptr, &fence->vk_semaphore));

	return fence;
}

void RHIDeviceVulkan::destroy_fence(RHIFence *fence) {
	api.destroySemaphore(fence->vk_semaphore, nullptr);
	delete fence;
}

uint64_t RHIDeviceVulkan::get_fence_value(RHIFence *fence) {
	uint64_t value;
	VK_CHECK(api.getSemaphoreCounterValue(fence->vk_semaphore, &value));
	return value;
}

void RHIDeviceVulkan::wait_fences(RHIFence **fence, uint64_t *value, uint32_t count) {
	std::vector<VkSemaphore> semaphores(count, VK_NULL_HANDLE);
	for (uint32_t i = 0; i < count; i++) {
		semaphores[i] = fence[i]->vk_semaphore;
	}
	VkSemaphoreWaitInfo wait_info;
	wait_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO;
	wait_info.pNext = NULL;
	wait_info.flags = 0;
	wait_info.semaphoreCount = 1;
	wait_info.pSemaphores = semaphores.data();
	wait_info.pValues = value;
	VK_CHECK(api.waitSemaphores(&wait_info, UINT64_MAX));
}

void RHIDeviceVulkan::signal_fence(RHIFence *fence, uint64_t value) {
	VkSemaphoreSignalInfo sig_info;
	sig_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SIGNAL_INFO;
	sig_info.pNext = nullptr;
	sig_info.semaphore = fence->vk_semaphore;
	sig_info.value = value;

	VK_CHECK(api.signalSemaphore(&sig_info));
}
// struct RHIBufferVulkan : public RHIBuffer {
// 	VkBuffer buffer = VK_NULL_HANDLE;
// 	VmaAllocation alloc = nullptr;
// 	uint64_t size = 0;
// 	VkBufferView view = VK_NULL_HANDLE; // For texel buffers.
// };

// RHIBuffer *RHIDeviceVulkan::create_buffer(uint64_t size, uint32_t usage, RHIMemoryType alloc_type, RHIFormat format) {
// 	VkBufferCreateInfo create_info = {};
// 	create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
// 	create_info.size = size;
// 	create_info.usage = usage;
// 	create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

// 	VmaAllocationCreateInfo alloc_create_info = {};
// 	switch (alloc_type) {
// 		case RHIMemoryType::CPU: {
// 			bool is_src = usage & (uint32_t)RHIBufferUsage::TRANSFER_SRC;
// 			bool is_dst = usage & (uint32_t)RHIBufferUsage::TRANSFER_DST;
// 			if (is_src && !is_dst) {
// 				// Looks like a staging buffer: CPU maps, writes sequentially, then GPU copies to VRAM.
// 				alloc_create_info.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
// 			}
// 			if (is_dst && !is_src) {
// 				// Looks like a readback buffer: GPU copies from VRAM, then CPU maps and reads.
// 				alloc_create_info.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT;
// 			}
// 			alloc_create_info.requiredFlags = (VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
// 		} break;
// 		case RHIMemoryType::GPU: {
// 			alloc_create_info.preferredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
// 		} break;
// 	}

// 	RHIBufferVulkan *buffer = new RHIBufferVulkan{};
// 	VmaAllocationInfo alloc_info = {};
// 	buffer->size = size;

// 	VK_CHECK_RESULT(vmaCreateBuffer(vma_allocator, &create_info, &alloc_create_info, &buffer->buffer, &buffer->alloc, nullptr));

// 	if (format != RHIFormat::MAX) {
// 		VkBufferViewCreateInfo view_create_info = {};
// 		view_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
// 		view_create_info.buffer = buffer->buffer;
// 		view_create_info.format = Vulkan::get_format(format);
// 		view_create_info.range = alloc_info.size;

// 		VK_CHECK_RESULT(api.createBufferView(&view_create_info, nullptr, &buffer->view));
// 	}
// 	return buffer;
// }

// void RHIDeviceVulkan::destroy_buffer(RHIBuffer *b) {
// 	RHIBufferVulkan *buffer = static_cast<RHIBufferVulkan *>(b);
// 	if (buffer->view) {
// 		api.destroyBufferView(buffer->view, nullptr);
// 	}
// 	vmaDestroyBuffer(vma_allocator, buffer->buffer, nullptr);
// 	delete buffer;
// }

// uint8_t *RHIDeviceVulkan::map_buffer_memory(RHIBuffer *b) {
// 	RHIBufferVulkan *buffer = static_cast<RHIBufferVulkan *>(b);
// 	void *data_ptr = nullptr;
// 	VK_CHECK_RESULT(vmaMapMemory(vma_allocator, buffer->alloc, &data_ptr));
// 	return (uint8_t *)data_ptr;
// }

// void RHIDeviceVulkan::unmap_buffer_memory(RHIBuffer *b) {
// 	RHIBufferVulkan *buffer = static_cast<RHIBufferVulkan *>(b);
// 	vmaUnmapMemory(vma_allocator, buffer->alloc);
// }
