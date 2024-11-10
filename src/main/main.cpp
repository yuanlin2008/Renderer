#include "SDL3/SDL.h"
#include "SDL3/SDL_vulkan.h"
#include "rhi_context_vulkan.h"

#include <iostream>

static RHIContext *context = nullptr;
static RHIDevice *device = nullptr;
static RHICommandQueue *queue = nullptr;
static SDL_Window *main_window = nullptr;
static RHISwapChain *swapchain = nullptr;
static bool quit = false;

void init() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Vulkan_LoadLibrary(nullptr);
	main_window = SDL_CreateWindow("test", 1024, 768, SDL_WINDOW_VULKAN);
	context = new RHIContextVulkan();
	device = context->create_device();
	queue = device->get_command_queue(RHICommandQueueType::Graphics);
	swapchain = device->create_swapchain(main_window, queue, RHIFormat::R8G8B8A8_UNORM, 3);
}

void shutdown() {
	device->destroy_swapchain(swapchain);
	context->destroy_device(device);
	delete context;
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