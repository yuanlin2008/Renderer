#include "engine.h"
#include "platform_sdl.h"

#include <iostream>

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