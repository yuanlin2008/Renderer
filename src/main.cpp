#include "SDL.h"
#include "SDL_vulkan.h"
#include <stdexcept>
#include "rhi.h"
#include "spdlog/spdlog.h"

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Renderer",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          800, 600,
                                          SDL_WINDOW_VULKAN);

    RHI rhi;
    rhi.init();

    SDL_Event e;
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
        }
    }

    rhi.close();
    SDL_DestroyWindow(window);

    return 0;
}