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

	/******************************
	 * Command Queue.
	 ******************************/
	virtual RHICommandQueue *get_command_queue(RHICommandQueueType type) override;

	/******************************
	 * Command Pool.
	 ******************************/
	virtual RHICommandPool *create_command_pool(RHICommandQueueType type) override;
	virtual void destroy_command_pool(RHICommandPool *pool) override;
	virtual void reset_command_pool(RHICommandPool *pool) override;

	/******************************
	 * Command Buffer.
	 ******************************/
	virtual RHICommandBuffer *create_command_buffer(RHICommandPool *pool) override;
	virtual void destroy_command_buffer(RHICommandPool *pool, RHICommandBuffer *cb) override;
	virtual void begin_command_buffer(RHICommandBuffer *cmd_buffer) override;
	virtual void end_command_buffer(RHICommandBuffer *cmd_buffer) override;

	/******************************
	 * Swapchain.
	 ******************************/
	virtual RHISwapChain *create_swapchain(SDL_Window *window, RHICommandQueue *queue, RHIFormat format, uint32_t count) override;
	virtual void destroy_swapchain(RHISwapChain *swapchain) override;

	/******************************
	 * Fence
	 ******************************/
	virtual RHIFence *create_fence(uint64_t init_value) override;
	virtual void destroy_fence(RHIFence *fence) override;
	virtual uint64_t get_fence_value(RHIFence *fence) override;
	virtual void wait_fences(RHIFence **fence, uint64_t *value, uint32_t count) override;
	virtual void signal_fence(RHIFence *fence, uint64_t value) override;

private:
	RHIContextVulkan *context = nullptr;
	vkb::Device device;
	vkb::DispatchTable api;
	VmaAllocator vma_allocator;
	RHICommandQueue *queue = nullptr;
};