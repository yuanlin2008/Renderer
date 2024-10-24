#pragma once

#include "rhi_context.h"

struct PlatformWindow {};

class Platform {
public:
	Platform();
	~Platform();
	static Platform *get_instance();

	virtual RHIContext *get_rhi_context() = 0;

	virtual PlatformWindow *create_window(const char *title, int width, int height) = 0;
	virtual void destroy_window(PlatformWindow *window) = 0;

	virtual RHISurface *create_surface(PlatformWindow *window) = 0;
	virtual void destroy_surface(RHISurface *surface) = 0;
};