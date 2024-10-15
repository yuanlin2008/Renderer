#include "renderer.h"
#include "context.h"
#include "device.h"
#include "command_buffer.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_vulkan.h"

#include <string>
#include <stdexcept>
#include "spdlog/spdlog.h"

Renderer::Renderer()
{
    SDL_Init(SDL_INIT_VIDEO);
    _window = SDL_CreateWindow(
        "Renderer",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        800,
        600,
        SDL_WINDOW_VULKAN);

    _context = new Context();

    SDL_Vulkan_CreateSurface(_window, _context->instance(), &_surface);

    _device = new Device(_context, _surface);

    vkb::SwapchainBuilder builder{_device->device()};
    _swapChain = builder
                     .add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
                     .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
                     .build()
                     .value();
    _swapChainImages = _swapChain.get_images().value();
    initFrames();
}

Renderer::~Renderer()
{
    _device->waitIdle();
    clearFrames();
    vkb::destroy_swapchain(_swapChain);
    _context->table().destroySurfaceKHR(_surface, nullptr);
    SDL_DestroyWindow(_window);
    delete _device;
    delete _context;
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

void Renderer::initFrames()
{
    for (int i = 0; i < FRAME_OVERLAP; i++)
    {
        auto &frame = _frames[i];
        frame.renderCompletedFence = _device->createFence(true);
        frame.renderCompletedSemaphore = _device->createSemaphore();
        frame.imageAcquiredSemaphore = _device->createSemaphore();
        frame.cmdPool = _device->createCommandPool(VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, _device->graphicsQueueIndex());
        frame.cmdBuffer = _device->createCommandBuffer(frame.cmdPool);
    }
}

void Renderer::clearFrames()
{
    for (int i = 0; i < FRAME_OVERLAP; i++)
    {
        auto &frame = _frames[i];
        _device->destroyFence(frame.renderCompletedFence);
        _device->destroySemaphore(frame.renderCompletedSemaphore);
        _device->destroySemaphore(frame.imageAcquiredSemaphore);
        _device->destroyCommandBuffer(frame.cmdPool, frame.cmdBuffer);
        _device->destroyCommandPool(frame.cmdPool);
    }
}

void Renderer::draw()
{
    const auto &frame = _frames[_frameNumber % FRAME_OVERLAP];
    // Wait until the gpu has finished rendering the last frame.
    _device->table().waitForFences(1, &frame.renderCompletedFence, true, 1000000000);
    _device->table().resetFences(1, &frame.renderCompletedFence);

    // request image from the swapchain
    uint32_t scIndex;
    _device->table().acquireNextImageKHR(_swapChain, 1000000000, frame.imageAcquiredSemaphore, nullptr, &scIndex);

    auto cmd = frame.cmdBuffer;

    cmd->reset(0);

    cmd->begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    cmd->transitionImage(_swapChainImages[scIndex],
                         VK_IMAGE_LAYOUT_UNDEFINED,
                         VK_IMAGE_LAYOUT_GENERAL);

    float flash = std::abs(std::sin(_frameNumber / 120.f));
    VkClearColorValue clearClr = {{0.f, 0.f, flash, 1.f}};
    cmd->clearColorImage(_swapChainImages[scIndex], VK_IMAGE_LAYOUT_GENERAL, clearClr);
    cmd->transitionImage(_swapChainImages[scIndex],
                         VK_IMAGE_LAYOUT_GENERAL,
                         VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

    cmd->end();

    // submit to the queue.
    cmd->submit(_device->graphicsQueue(),
                frame.imageAcquiredSemaphore,
                frame.renderCompletedSemaphore,
                frame.renderCompletedFence);

    // present
    _device->present(_device->graphicsQueue(),
                     &_swapChain.swapchain, scIndex,
                     frame.renderCompletedSemaphore);

    // inc the frame number.
    _frameNumber++;
}
