#include "SDL3/SDL.h"
#include "SDL3/SDL_vulkan.h"
#include "engine.h"

#include <iostream>

int pmain() {
	SDL_Init(SDL_INIT_VIDEO);
}

int main(int argc, char *argv[]) {
	try {
		PlatformSDL platform;
		Engine::initialize();
		while (true) {
			platform.handle_events();
			if (!Engine::iteration())
				break;
		}
		Engine::shutdown();
		return 0;
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
}