#include "rhi_device_vulkan.h"

#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

RHIDeviceVulkan::RHIDeviceVulkan(RHIContextVulkan *context, vkb::PhysicalDevice &physical_device) {
	// create device.
	vkb::DeviceBuilder builder(physical_device);
	auto ret = builder.build();
	if (!ret) {
		throw std::runtime_error(
				std::string("Failed to create device. Error: ") +
				ret.error().message());
	}
	device = ret.value();
	device_funcs = device.make_table();

	// create vma.
	VmaVulkanFunctions funcs = {
		.vkGetInstanceProcAddr = context->get_instance().fp_vkGetInstanceProcAddr,
		.vkGetDeviceProcAddr = context->get_instance().fp_vkGetDeviceProcAddr
	};
	VmaAllocatorCreateInfo alloc_info = {
		.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
		.physicalDevice = physical_device,
		.device = device,
		.pVulkanFunctions = &funcs,
		.instance = context->get_instance()
	};
	VK_CHECK_RESULT(vmaCreateAllocator(&alloc_info, &vma_allocator));
}

RHIDeviceVulkan::~RHIDeviceVulkan() {
	vmaDestroyAllocator(vma_allocator);
	vkb::destroy_device(device);
}

struct RHIBufferVulkan : public RHIBuffer {
	VkBuffer buffer = VK_NULL_HANDLE;
	VmaAllocation alloc = nullptr;
	uint64_t size = 0;
	VkBufferView view = VK_NULL_HANDLE; // For texel buffers.
};

RHIBuffer *RHIDeviceVulkan::create_buffer(uint64_t size, uint32_t usage, RHIMemoryType alloc_type, RHIFormat format) {
	VkBufferCreateInfo create_info = {};
	create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	create_info.size = size;
	create_info.usage = usage;
	create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VmaAllocationCreateInfo alloc_create_info = {};
	switch (alloc_type) {
		case RHIMemoryType::CPU: {
			bool is_src = usage & (uint32_t)RHIBufferUsage::TRANSFER_SRC;
			bool is_dst = usage & (uint32_t)RHIBufferUsage::TRANSFER_DST;
			if (is_src && !is_dst) {
				// Looks like a staging buffer: CPU maps, writes sequentially, then GPU copies to VRAM.
				alloc_create_info.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
			}
			if (is_dst && !is_src) {
				// Looks like a readback buffer: GPU copies from VRAM, then CPU maps and reads.
				alloc_create_info.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT;
			}
			alloc_create_info.requiredFlags = (VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		} break;
		case RHIMemoryType::GPU: {
			alloc_create_info.preferredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		} break;
	}

	RHIBufferVulkan *buffer = new RHIBufferVulkan{};
	VmaAllocationInfo alloc_info = {};
	buffer->size = size;

	VK_CHECK_RESULT(vmaCreateBuffer(vma_allocator, &create_info, &alloc_create_info, &buffer->buffer, &buffer->alloc, nullptr));

	if (format != RHIFormat::MAX) {
		VkBufferViewCreateInfo view_create_info = {};
		view_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
		view_create_info.buffer = buffer->buffer;
		view_create_info.format = Vulkan::get_format(format);
		view_create_info.range = alloc_info.size;

		VK_CHECK_RESULT(device_funcs.createBufferView(&view_create_info, nullptr, &buffer->view));
	}
	return buffer;
}

void RHIDeviceVulkan::destroy_buffer(RHIBuffer *b) {
	RHIBufferVulkan *buffer = static_cast<RHIBufferVulkan *>(b);
	if (buffer->view) {
		device_funcs.destroyBufferView(buffer->view, nullptr);
	}
	vmaDestroyBuffer(vma_allocator, buffer->buffer, nullptr);
	delete buffer;
}

uint8_t *RHIDeviceVulkan::map_buffer_memory(RHIBuffer *b) {
	RHIBufferVulkan *buffer = static_cast<RHIBufferVulkan *>(b);
	void *data_ptr = nullptr;
	VK_CHECK_RESULT(vmaMapMemory(vma_allocator, buffer->alloc, &data_ptr));
	return (uint8_t *)data_ptr;
}

void RHIDeviceVulkan::unmap_buffer_memory(RHIBuffer *b) {
	RHIBufferVulkan *buffer = static_cast<RHIBufferVulkan *>(b);
	vmaUnmapMemory(vma_allocator, buffer->alloc);
}
