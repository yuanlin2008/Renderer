#include "platform_sdl.h"

#include "SDL2/SDL.h"

void PlatformSDL::initialize() {
	SDL_Init(SDL_INIT_VIDEO);
}

void PlatformSDL::shutdown() {
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