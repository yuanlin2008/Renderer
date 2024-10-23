#pragma once

#include "rhi_context_vulkan.h"

#include "VkBootstrap.h"
#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1
#include "vk_mem_alloc.h"

class RHIContextVulkan;

class RHIDeviceVulkan : public RHIDevice {
public:
	RHIDeviceVulkan(RHIContextVulkan *context, vkb::PhysicalDevice &physical_device);
	virtual ~RHIDeviceVulkan() override;

private:
	vkb::Device device;
	vkb::DispatchTable device_funcs;
	VmaAllocator vma_allocator;
};