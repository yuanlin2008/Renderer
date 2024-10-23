#include "platform_sdl.h"

#include "engine.h"
#include "rhi_context_vulkan.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_vulkan.h"

PlatformSDL::PlatformSDL() {
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Vulkan_LoadLibrary(nullptr);
	unsigned int ext_count;
	SDL_Vulkan_GetInstanceExtensions(nullptr, &ext_count, nullptr);
	std::vector<const char *> extensions(ext_count);
	SDL_Vulkan_GetInstanceExtensions(nullptr, &ext_count, extensions.data());
	rhi_context = new RHIContextVulkan(extensions);
}

PlatformSDL::~PlatformSDL() {
	delete rhi_context;
	SDL_Vulkan_UnloadLibrary();
	SDL_Quit();
}

void PlatformSDL::handle_events() {
	SDL_Event e;

	// Handle events on queue
	while (SDL_PollEvent(&e) != 0) {
		// close the window when user alt-f4s or clicks the X button
		if (e.type == SDL_QUIT) {
			Engine::quit();
		}
	}
}

RHIContext *PlatformSDL::get_rhi_context() {
	return rhi_context;
}

struct PlatformWindowSDL : public PlatformWindow {
	SDL_Window *window;
};

PlatformWindow *PlatformSDL::create_window(const char *title, int width, int height) {
	PlatformWindowSDL *window = new PlatformWindowSDL{
		.window = SDL_CreateWindow(title,
				SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED,
				width,
				height,
				SDL_WINDOW_VULKAN)
	};
	return window;
}

void PlatformSDL::destroy_window(PlatformWindow *window) {
	SDL_DestroyWindow(static_cast<PlatformWindowSDL *>(window)->window);
	delete window;
}

RHISurface *PlatformSDL::create_surface(PlatformWindow *window) {
	RHISurfaceVulkan *surface = new RHISurfaceVulkan();
	SDL_Vulkan_CreateSurface(
			static_cast<PlatformWindowSDL *>(window)->window,
			static_cast<RHIContextVulkan *>(rhi_context)->get_instance(),
			&surface->surface);
	return surface;
}

void PlatformSDL::destroy_surface(RHISurface *surface) {
	static_cast<RHIContextVulkan *>(rhi_context)
			->get_instance_funcs()
			.destroySurfaceKHR(static_cast<RHISurfaceVulkan *>(surface)->surface, nullptr);
	delete surface;
}