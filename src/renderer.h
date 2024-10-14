#pragma once

#include "VkBootstrap.h"

class Context;
class Device;
class CommandBuffer;

class Renderer
{
public:
    Renderer();
    Renderer(const Renderer &) = delete;
    void operator=(const Renderer &) = delete;
    ~Renderer();

    void run();

private:
    void initFrames();
    void clearFrames();

    void draw();

    Context *_context;
    Device *_device;

    struct SDL_Window *_window{nullptr};
    VkSurfaceKHR _surface;
    vkb::Swapchain _swapChain;
    std::vector<VkImage> _swapChainImages;

    static constexpr unsigned int FRAME_OVERLAP = 2;
    struct Frame
    {
        VkFence renderCompletedFence;
        VkSemaphore renderCompletedSemaphore;
        VkSemaphore imageAcquiredSemaphore;
        VkCommandPool cmdPool;
        CommandBuffer *cmdBuffer;
    } _frames[FRAME_OVERLAP];
    int _frameNumber{0};
};