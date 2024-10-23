#include "engine.h"

#include "platform.h"

static PlatformWindow *main_window = nullptr;
static RHISurface *main_surface = nullptr;
static RHIDevice *rhi_device = nullptr;

void Engine::initialize() {
	main_window = Platform::get_instance()->create_window("Engine", 800, 600);
	main_surface = Platform::get_instance()->create_surface(main_window);
	rhi_device = Platform::get_instance()->get_rhi_context()->create_device(main_surface);
}

static bool quit_request = false;
bool Engine::iteration() {
	if (quit_request)
		return false;
	return true;
}

void Engine::shutdown() {
	Platform::get_instance()->get_rhi_context()->destroy_device(rhi_device);
	Platform::get_instance()->destroy_surface(main_surface);
	Platform::get_instance()->destroy_window(main_window);
}

void Engine::quit() {
	quit_request = true;
}