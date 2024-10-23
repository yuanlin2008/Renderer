#include "rhi_device_vulkan.h"

#include "rhi_context_vulkan.h"

#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"
#include <stdexcept>

RHIDeviceVulkan::RHIDeviceVulkan(RHIContextVulkan *context, vkb::PhysicalDevice &physical_device) {
	// create device.
	vkb::DeviceBuilder deviceBuilder(physical_device);
	device = deviceBuilder.build().value();
	device_funcs = device.make_table();

	// create vma.
	VmaVulkanFunctions vmaVKFuncs = {
		.vkGetInstanceProcAddr = context->get_instance().fp_vkGetInstanceProcAddr,
		.vkGetDeviceProcAddr = context->get_instance().fp_vkGetDeviceProcAddr
	};
	VmaAllocatorCreateInfo allocatorInfo = {
		.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
		.physicalDevice = physical_device,
		.device = device,
		.pVulkanFunctions = &vmaVKFuncs,
		.instance = context->get_instance()
	};
	vmaCreateAllocator(&allocatorInfo, &vma_allocator);
}

RHIDeviceVulkan::~RHIDeviceVulkan() {
	vmaDestroyAllocator(vma_allocator);
	vkb::destroy_device(device);
}