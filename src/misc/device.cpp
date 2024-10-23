#include "device.h"
#include "command_buffer.h"
#include "context.h"
#include "spdlog/spdlog.h"

#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

Device::Device(Context *ctx, VkSurfaceKHR surface) {
	// select a gpu.
	VkPhysicalDeviceVulkan13Features features13{
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
		.synchronization2 = true,
		.dynamicRendering = true,
	};

	VkPhysicalDeviceVulkan12Features features12{
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
		.descriptorIndexing = true,
		.bufferDeviceAddress = true,
	};

	vkb::PhysicalDeviceSelector pdSelector(ctx->instance());
	vkb::PhysicalDevice phyDevice = pdSelector.set_minimum_version(1, 3)
											.set_required_features_13(features13)
											.set_required_features_12(features12)
											.set_surface(surface)
											.select()
											.value();
	SPDLOG_INFO("Physical Device: {}", phyDevice.name);

	// create device.
	vkb::DeviceBuilder deviceBuilder(phyDevice);
	_device = deviceBuilder.build().value();
	_table = _device.make_table();
	_graphicsQueue = _device.get_queue(vkb::QueueType::graphics).value();
	_graphicsQueueFamily =
			_device.get_queue_index(vkb::QueueType::graphics).value();

	VmaVulkanFunctions vmaVKFuncs = {
		.vkGetInstanceProcAddr = ctx->instance().fp_vkGetInstanceProcAddr,
		.vkGetDeviceProcAddr = ctx->instance().fp_vkGetDeviceProcAddr
	};
	VmaAllocatorCreateInfo allocatorInfo = {
		.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
		.physicalDevice = phyDevice,
		.device = _device,
		.pVulkanFunctions = &vmaVKFuncs,
		.instance = ctx->instance()
	};
	vmaCreateAllocator(&allocatorInfo, &_allocator);
}

Device::~Device() {
	vmaDestroyAllocator(_allocator);
	vkb::destroy_device(_device);
}

VkFence Device::createFence(bool signaled) {
	VkFenceCreateInfo fenceCI{ .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
		.pNext = nullptr,
		.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT
						  : (VkFenceCreateFlags)0 };
	VkFence fence;
	_table.createFence(&fenceCI, nullptr, &fence);
	return fence;
}

void Device::destroyFence(VkFence fence) {
	_table.destroyFence(fence, nullptr);
}

VkSemaphore Device::createSemaphore() {
	VkSemaphoreCreateInfo semaCI{ .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0 };
	VkSemaphore sem;
	_table.createSemaphore(&semaCI, nullptr, &sem);
	return sem;
}

void Device::destroySemaphore(VkSemaphore s) {
	_table.destroySemaphore(s, nullptr);
}

void Device::waitIdle() { _table.deviceWaitIdle(); }

VkCommandPool Device::createCommandPool(VkCommandPoolCreateFlags flags,
		uint32_t queueFamilyIndex) {
	VkCommandPoolCreateInfo cpci{
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.pNext = nullptr,
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		.queueFamilyIndex = queueFamilyIndex
	};
	VkCommandPool pool;
	_table.createCommandPool(&cpci, nullptr, &pool);
	return pool;
}

void Device::destroyCommandPool(VkCommandPool p) {
	_table.destroyCommandPool(p, nullptr);
}

CommandBuffer *Device::createCommandBuffer(VkCommandPool pool) {
	VkCommandBufferAllocateInfo cbai{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.pNext = nullptr,
		.commandPool = pool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = 1,
	};
	VkCommandBuffer cb;
	_table.allocateCommandBuffers(&cbai, &cb);
	return new CommandBuffer(_table, cb);
}

void Device::destroyCommandBuffer(VkCommandPool pool, CommandBuffer *cb) {
	auto handle = cb->handle();
	_table.freeCommandBuffers(pool, 1, &handle);
	delete cb;
}

void Device::present(VkQueue queue, VkSwapchainKHR *swapchain, uint32_t index,
		VkSemaphore wait) {
	VkPresentInfoKHR presentInfo{ .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		.pNext = nullptr,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &wait,
		.swapchainCount = 1,
		.pSwapchains = swapchain,
		.pImageIndices = &index };

	_table.queuePresentKHR(queue, &presentInfo);
}