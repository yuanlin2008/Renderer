#pragma once

#include "VkBootstrap.h"
#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1
#include "vk_mem_alloc.h"

class Context;
class CommandBuffer;

class Device
{
public:
    Device(Context *ctx, VkSurfaceKHR surface);
    Device(const Device &) = delete;
    void operator=(const Device &) = delete;
    ~Device();

    const vkb::Device &device() const { return _device; }
    const vkb::DispatchTable &table() const { return _table; }
    const VkQueue graphicsQueue() const { return _graphicsQueue; }
    const uint32_t graphicsQueueIndex() const { return _graphicsQueueFamily; }

    VkFence createFence(bool signaled);
    void destroyFence(VkFence fence);
    VkSemaphore createSemaphore();
    void destroySemaphore(VkSemaphore s);
    void waitIdle();

    VkCommandPool createCommandPool(VkCommandPoolCreateFlags flags, uint32_t queueFamilyIndex);
    void destroyCommandPool(VkCommandPool p);

    CommandBuffer* createCommandBuffer(VkCommandPool pool);
    void destroyCommandBuffer(VkCommandPool pool, CommandBuffer* cb);

    void present(VkQueue queue, VkSwapchainKHR* swapchain, uint32_t index, VkSemaphore wait);
private:
    vkb::Device _device;
    vkb::DispatchTable _table;
    VmaAllocator _allocator;
    VkQueue _graphicsQueue;
    uint32_t _graphicsQueueFamily;
};