#include "platform_sdl.h"

#include "rhi_context_vulkan.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_vulkan.h"

void PlatformSDL::initialize() {
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Vulkan_LoadLibrary(nullptr);
	unsigned int ext_count;
	SDL_Vulkan_GetInstanceExtensions(nullptr, &ext_count, nullptr);
	std::vector<const char *> extensions(ext_count);
	SDL_Vulkan_GetInstanceExtensions(nullptr, &ext_count, extensions.data());
	rhi_context = new RHIContextVulkan(extensions);
}

void PlatformSDL::shutdown() {
	delete rhi_context;
	SDL_Vulkan_UnloadLibrary();
	SDL_Quit();
}

void PlatformSDL::run() {
	SDL_Event e;
	bool stopRendering = false;
	bool bQuit = false;

	// main loop
	while (!bQuit) {
		// Handle events on queue
		while (SDL_PollEvent(&e) != 0) {
			// close the window when user alt-f4s or clicks the X button
			if (e.type == SDL_QUIT)
				bQuit = true;

			if (e.type == SDL_WINDOWEVENT) {
				if (e.window.event == SDL_WINDOWEVENT_MINIMIZED) {
					stopRendering = true;
				}
				if (e.window.event == SDL_WINDOWEVENT_RESTORED) {
					stopRendering = false;
				}
			}
		}

		// do not draw if we are minimized
		if (stopRendering) {
			// throttle the speed to avoid the endless spinning
			//std::this_thread::sleep_for(std::chrono::milliseconds(100));
			continue;
		} else {
			//draw();
		}
	}
}

RHIContext *PlatformSDL::get_rhi_context() {
	return nullptr;
}

PlatformWindow *PlatformSDL::create_window() {
	return nullptr;
}

void PlatformSDL::destroy_window(PlatformWindow *window) {
}

RHISurface *PlatformSDL::create_surface(PlatformWindow *window) {
	return nullptr;
}

void PlatformSDL::destroy_surface(RHISurface *surface) {
}