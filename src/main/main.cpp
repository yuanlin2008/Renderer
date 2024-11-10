#include "SDL3/SDL.h"
#include "SDL3/SDL_vulkan.h"
#include "rhi_context_vulkan.h"

#include <iostream>

static SDL_Window *main_window = nullptr;
static RHIContext *rhi_context = nullptr;
static RHIDevice *rhi_device = nullptr;
static bool quit = false;

void init() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Vulkan_LoadLibrary(nullptr);
	main_window = SDL_CreateWindow("test", 1024, 768, SDL_WINDOW_VULKAN);
	rhi_context = new RHIContextVulkan();
	rhi_device = rhi_context->create_device();
}

void shutdown() {
	rhi_context->destroy_device(rhi_device);
	delete rhi_context;
	SDL_DestroyWindow(main_window);
	SDL_Vulkan_UnloadLibrary();
	SDL_Quit();
}

void handle_events() {
	SDL_Event e;
	// Handle events on queue
	while (SDL_PollEvent(&e) != 0) {
		// close the window when user alt-f4s or clicks the X button
		if (e.type == SDL_EVENT_QUIT) {
			quit = true;
			break;
		}
	}
}

void run() {
	while (!quit) {
		handle_events();
		// render.
	}
}

void pmain() {
	init();
	run();
	shutdown();
}

int main(int argc, char *argv[]) {
	try {
		pmain();
		return 0;
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
}