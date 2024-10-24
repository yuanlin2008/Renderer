#include "rhi_device_vulkan.h"

#include "rhi_context_vulkan.h"

#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"
#include <stdexcept>

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
	if (vmaCreateAllocator(&allocatorInfo, &vma_allocator) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create vma allocator.");
	}
}

RHIDeviceVulkan::~RHIDeviceVulkan() {
	vmaDestroyAllocator(vma_allocator);
	vkb::destroy_device(device);
}