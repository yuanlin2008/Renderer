#include "renderer.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_vulkan.h"

#include <string>
#include <stdexcept>
#include "spdlog/spdlog.h"

void Renderer::init()
{
    SDL_Init(SDL_INIT_VIDEO);
    _window = SDL_CreateWindow(
        "Renderer",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        800,
        600,
        SDL_WINDOW_VULKAN);

    initVulkan();
    createSwapChain();
    initFrames();
}

void Renderer::close()
{
    _devDT.deviceWaitIdle();
    clearFrames();
    destroySwapChain();
    closeVulkan();
    SDL_DestroyWindow(_window);
}

void Renderer::run()
{
    SDL_Event e;
    bool stopRendering = false;
    bool bQuit = false;

    // main loop
    while (!bQuit)
    {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            // close the window when user alt-f4s or clicks the X button
            if (e.type == SDL_QUIT)
                bQuit = true;

            if (e.type == SDL_WINDOWEVENT)
            {
                if (e.window.event == SDL_WINDOWEVENT_MINIMIZED)
                {
                    stopRendering = true;
                }
                if (e.window.event == SDL_WINDOWEVENT_RESTORED)
                {
                    stopRendering = false;
                }
            }
        }

        // do not draw if we are minimized
        if (stopRendering)
        {
            // throttle the speed to avoid the endless spinning
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        else
        {
            draw();
        }
    }
}

void Renderer::initVulkan()
{
    //
    // create instance.
    //
    vkb::InstanceBuilder instanceBuilder;
    auto instRet = instanceBuilder
                       .set_app_name("Renderer")
                       .request_validation_layers(true)
                       .use_default_debug_messenger()
                       .require_api_version(1, 3, 225)
                       .build();
    if (!instRet)
    {
        throw std::runtime_error(
            std::string("Failed to create Vulkan instance. Error: ") +
            instRet.error().message());
    }
    _instance = instRet.value();
    _instDT = _instance.make_table();

    SDL_Vulkan_CreateSurface(_window, _instance, &_surface);

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

    vkb::PhysicalDeviceSelector pdSelector(_instance);
    vkb::PhysicalDevice phyDevice = pdSelector
                                        .set_minimum_version(1, 3)
                                        .set_required_features_13(features13)
                                        .set_required_features_12(features12)
                                        .set_surface(_surface)
                                        .select()
                                        .value();
    SPDLOG_INFO("Physical Device: {}", phyDevice.name);

    //
    // create device.
    //
    vkb::DeviceBuilder deviceBuilder(phyDevice);
    _device = deviceBuilder.build().value();
    _devDT = _device.make_table();
    _graphicQueue = _device.get_queue(vkb::QueueType::graphics).value();
    _graphicQueueFamily = _device.get_queue_index(vkb::QueueType::graphics).value();
}

void Renderer::closeVulkan()
{
    vkb::destroy_device(_device);
    _instDT.destroySurfaceKHR(_surface, nullptr);
    vkb::destroy_instance(_instance);
}

void Renderer::createSwapChain()
{
    vkb::SwapchainBuilder builder{_device};
    _swapChain = builder
                     .add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
                     .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
                     .build()
                     .value();
    _swapChainImages = _swapChain.get_images().value();
    _swapChainImageViews = _swapChain.get_image_views().value();
}

void Renderer::destroySwapChain()
{
    _swapChain.destroy_image_views(_swapChainImageViews);
    vkb::destroy_swapchain(_swapChain);
}

void Renderer::initFrames()
{
    for (int i = 0; i < FRAME_OVERLAP; i++)
    {
        auto &frame = _frames[i];

        VkFenceCreateInfo fenceCI{
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_FENCE_CREATE_SIGNALED_BIT};
        _devDT.createFence(&fenceCI, nullptr, &frame.renderCompletedFence);

        VkSemaphoreCreateInfo semaCI{
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0};
        _devDT.createSemaphore(&semaCI, nullptr, &frame.renderCompletedSemaphore);
        _devDT.createSemaphore(&semaCI, nullptr, &frame.imageAcquiredSemaphore);

        VkCommandPoolCreateInfo cpci{
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex = _graphicQueueFamily,
        };
        _devDT.createCommandPool(&cpci, nullptr, &frame.cmdPool);

        VkCommandBufferAllocateInfo cbai{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext = nullptr,
            .commandPool = frame.cmdPool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1,
        };
        _devDT.allocateCommandBuffers(&cbai, &frame.cmdBuffer);
    }
}

void Renderer::clearFrames()
{
    for (int i = 0; i < FRAME_OVERLAP; i++)
    {
        auto &frame = _frames[i];
        _devDT.destroyFence(frame.renderCompletedFence, nullptr);
        _devDT.destroySemaphore(frame.renderCompletedSemaphore, nullptr);
        _devDT.destroySemaphore(frame.imageAcquiredSemaphore, nullptr);
        _devDT.destroyCommandPool(frame.cmdPool, nullptr);
    }
}

static void transitionImage(const vkb::DispatchTable &dt, VkCommandBuffer cmd, VkImage image, VkImageLayout currentLayout, VkImageLayout newLayout)
{
    VkImageMemoryBarrier2 imageBarrier{.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2};
    imageBarrier.pNext = nullptr;

    imageBarrier.srcStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
    imageBarrier.srcAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT;
    imageBarrier.dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
    imageBarrier.dstAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT | VK_ACCESS_2_MEMORY_READ_BIT;

    imageBarrier.oldLayout = currentLayout;
    imageBarrier.newLayout = newLayout;

    VkImageAspectFlags aspectMask = (newLayout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
    imageBarrier.subresourceRange = {
        .aspectMask = aspectMask,
        .baseMipLevel = 0,
        .levelCount = VK_REMAINING_MIP_LEVELS,
        .baseArrayLayer = 0,
        .layerCount = VK_REMAINING_MIP_LEVELS};
    imageBarrier.image = image;

    VkDependencyInfo depInfo{};
    depInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    depInfo.pNext = nullptr;

    depInfo.imageMemoryBarrierCount = 1;
    depInfo.pImageMemoryBarriers = &imageBarrier;

    dt.cmdPipelineBarrier2(cmd, &depInfo);
}

void Renderer::draw()
{
    const auto &frame = _frames[_frameNumber % FRAME_OVERLAP];
    // Wait until the gpu has finished rendering the last frame.
    _devDT.waitForFences(1, &frame.renderCompletedFence, true, 1000000000);
    _devDT.resetFences(1, &frame.renderCompletedFence);

    // request image from the swapchain
    uint32_t scIndex;
    _devDT.acquireNextImageKHR(_swapChain, 1000000000, frame.imageAcquiredSemaphore, nullptr, &scIndex);

    auto cmd = frame.cmdBuffer;

    // now that we are sure that the commands finished executing, we can safely
    // reset the command buffer to begin recording again.
    _devDT.resetCommandBuffer(cmd, 0);

    // begin the command buffer recording. We will use this command buffer exactly once, so we want to let vulkan know that
    VkCommandBufferBeginInfo cmdBeginInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = nullptr,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
        .pInheritanceInfo = nullptr};

    _devDT.beginCommandBuffer(cmd, &cmdBeginInfo);

    transitionImage(_devDT, cmd, _swapChainImages[scIndex], VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL);
    float flash = std::abs(std::sin(_frameNumber / 120.f));
    VkClearColorValue clearClr = {{0.f, 0.f, flash, 1.f}};
    VkImageSubresourceRange clearRange{
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .baseMipLevel = 0,
        .levelCount = VK_REMAINING_MIP_LEVELS,
        .baseArrayLayer = 0,
        .layerCount = VK_REMAINING_MIP_LEVELS};
    _devDT.cmdClearColorImage(cmd, _swapChainImages[scIndex], VK_IMAGE_LAYOUT_GENERAL, &clearClr, 1, &clearRange);
    transitionImage(_devDT, cmd, _swapChainImages[scIndex], VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

    _devDT.endCommandBuffer(cmd);

    // submit to the queue.
    VkCommandBufferSubmitInfo info{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
        .pNext = nullptr,
        .commandBuffer = cmd,
        .deviceMask = 0};
    VkSemaphoreSubmitInfo waitInfo{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .pNext = nullptr,
        .semaphore = frame.imageAcquiredSemaphore,
        .value = 1,
        .stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
        .deviceIndex = 0};
    VkSemaphoreSubmitInfo sigInfo{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .pNext = nullptr,
        .semaphore = frame.renderCompletedSemaphore,
        .value = 1,
        .stageMask = VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT,
        .deviceIndex = 0};
    VkSubmitInfo2 submitInfo{
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
        .pNext = nullptr,
        .waitSemaphoreInfoCount = 1,
        .pWaitSemaphoreInfos = &waitInfo,
        .commandBufferInfoCount = 1,
        .pCommandBufferInfos = &info,
        .signalSemaphoreInfoCount = 1,
        .pSignalSemaphoreInfos = &sigInfo};
    _devDT.queueSubmit2(_graphicQueue, 1, &submitInfo, frame.renderCompletedFence);

    // present
    VkPresentInfoKHR presentInfo{
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext = nullptr,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &frame.renderCompletedSemaphore,
        .swapchainCount = 1,
        .pSwapchains = &_swapChain.swapchain,
        .pImageIndices = &scIndex};

    _devDT.queuePresentKHR(_graphicQueue, &presentInfo);

    // inc the frame number.
    _frameNumber++;
}