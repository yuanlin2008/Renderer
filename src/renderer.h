#pragma once

#include "VkBootstrap.h"

class Renderer
{
public:
    void init();
    void run();
    void close();

private:
    void createWindow();
    void destroyWindow();
    void draw();

    vkb::Instance _instance;
    vkb::Device _device;
    vkb::DispatchTable _deviceFN;
    VkQueue _graphicQueue;
    int _frameNumber{0};
    struct SDL_Window *_window{nullptr};
};