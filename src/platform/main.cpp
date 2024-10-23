#include "platform_sdl.h"

int main(int argc, char *argv[]) {
	PlatformSDL platform;
	platform.initialize();
	platform.run();
	platform.shutdown();
	return 0;
}