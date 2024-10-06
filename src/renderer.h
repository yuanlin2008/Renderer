#pragma once

#include "VkBootstrap.h"

class Renderer
{
public:
    void init();
    void run();
    void close();

private:
    void initVulkan();
    void closeVulkan();
    void createSwapChain();
    void destroySwapChain();
    void initFrames();
    void clearFrames();
    void draw();

    int _frameNumber{0};
    vkb::Instance _instance;
    vkb::InstanceDispatchTable _instDT;
    vkb::Device _device;
    vkb::DispatchTable _devDT;
    VkQueue _graphicQueue;
    uint32_t _graphicQueueFamily;

    struct SDL_Window *_window{nullptr};
    VkSurfaceKHR _surface;
    vkb::Swapchain _swapChain;
    std::vector<VkImage> _swapChainImages;
    std::vector<VkImageView> _swapChainImageViews;

    static constexpr unsigned int FRAME_OVERLAP = 2;
    struct Frame
    {
        VkFence renderCompletedFence;
        VkSemaphore renderCompletedSemaphore;
        VkSemaphore imageAcquiredSemaphore;
        VkCommandPool cmdPool;
        VkCommandBuffer cmdBuffer;
    } _frames[FRAME_OVERLAP];
};