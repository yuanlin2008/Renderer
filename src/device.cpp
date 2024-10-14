#include "device.h"
#include "context.h"
#include "command_buffer.h"
#include "spdlog/spdlog.h"

Device::Device(Context *ctx, VkSurfaceKHR surface)
{
    //
    // select a gpu.
    //

    // vulkan 1.3 features
    VkPhysicalDeviceVulkan13Features features13{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
        .synchronization2 = true,
        .dynamicRendering = true,
    };

    // vulkan 1.2 features
    VkPhysicalDeviceVulkan12Features features12{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
        .descriptorIndexing = true,
        .bufferDeviceAddress = true,
    };

    vkb::PhysicalDeviceSelector pdSelector(ctx->instance());
    vkb::PhysicalDevice phyDevice = pdSelector
                                        .set_minimum_version(1, 3)
                                        .set_required_features_13(features13)
                                        .set_required_features_12(features12)
                                        .set_surface(surface)
                                        .select()
                                        .value();
    SPDLOG_INFO("Physical Device: {}", phyDevice.name);

    //
    // create device.
    //
    vkb::DeviceBuilder deviceBuilder(phyDevice);
    _device = deviceBuilder.build().value();
    _table = _device.make_table();
    _graphicsQueue = _device.get_queue(vkb::QueueType::graphics).value();
    _graphicsQueueFamily = _device.get_queue_index(vkb::QueueType::graphics).value();
}

Device::~Device()
{
    vkb::destroy_device(_device);
}

VkFence Device::createFence(bool signaled)
{
    VkFenceCreateInfo fenceCI{
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0};
    VkFence fence;
    _table.createFence(&fenceCI, nullptr, &fence);
    return fence;
}

void Device::destroyFence(VkFence fence)
{
    _table.destroyFence(fence, nullptr);
}

VkSemaphore Device::createSemaphore()
{
    VkSemaphoreCreateInfo semaCI{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0};
    VkSemaphore sem;
    _table.createSemaphore(&semaCI, nullptr, &sem);
    return sem;
}

void Device::destroySemaphore(VkSemaphore s)
{
    _table.destroySemaphore(s, nullptr);
}

VkCommandPool Device::createCommandPool(VkCommandPoolCreateFlags flags, uint32_t queueFamilyIndex)
{
    VkCommandPoolCreateInfo cpci{
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = queueFamilyIndex};
    VkCommandPool pool;
    _table.createCommandPool(&cpci, nullptr, &pool);
    return pool;
}

void Device::destroyCommandPool(VkCommandPool p)
{
    _table.destroyCommandPool(p, nullptr);
}

CommandBuffer *Device::createCommandBuffer(VkCommandPool pool)
{
    VkCommandBufferAllocateInfo cbai{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = nullptr,
        .commandPool = pool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    };
    VkCommandBuffer cb;
    _table.allocateCommandBuffers(&cbai, &cb);
    return new CommandBuffer(cb);
}

void Device::destroyCommandBuffer(VkCommandPool pool, CommandBuffer *cb)
{
    auto handle = cb->handle();
    _table.freeCommandBuffers(pool, 1, &handle);
    delete cb;
}
