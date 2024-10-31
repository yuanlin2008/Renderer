#pragma once

#include "rhi_context_vulkan.h"

#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1
#include "vk_mem_alloc.h"

class RHIContextVulkan;

class RHIDeviceVulkan : public RHIDevice {
public:
	RHIDeviceVulkan(RHIContextVulkan *context, vkb::PhysicalDevice &physical_device);
	virtual ~RHIDeviceVulkan() override;

	virtual RHIBuffer *create_buffer(uint64_t size, uint32_t usage, RHIMemoryType alloc_type, RHIFormat format) override;
	virtual void destroy_buffer(RHIBuffer *buffer) override;
	virtual uint8_t *map_buffer_memory(RHIBuffer *buffer) override;
	virtual void unmap_buffer_memory(RHIBuffer *buffer) override;

private:
	vkb::Device device;
	vkb::DispatchTable device_funcs;
	VmaAllocator vma_allocator;
};