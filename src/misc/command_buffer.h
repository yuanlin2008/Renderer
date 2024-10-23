#pragma once

#include "VkBootstrap.h"

class CommandBuffer
{
public:
    CommandBuffer(vkb::DispatchTable &t, VkCommandBuffer cb);
    CommandBuffer(const CommandBuffer &) = delete;
    void operator=(const CommandBuffer &) = delete;
    ~CommandBuffer();

    VkCommandBuffer handle() const { return _cb; }

    void reset(VkCommandBufferResetFlags flags);
    void begin(VkCommandBufferUsageFlags flags);
    void end();
    void submit(VkQueue queue, VkSemaphore wait, VkSemaphore signal, VkFence singalFence);

    void transitionImage(VkImage image, VkImageLayout currentLayout, VkImageLayout newLayout);
    void clearColorImage(VkImage image, VkImageLayout layout , VkClearColorValue c);
private:
    vkb::DispatchTable &_table;
    VkCommandBuffer _cb{VK_NULL_HANDLE};
};