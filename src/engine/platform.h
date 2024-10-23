#pragma once

#include "rhi_context.h"

struct PlatformWindow {};

class Platform {
public:
	Platform();
	~Platform();
	static Platform *get_instance();

	virtual void initialize() = 0;
	virtual void shutdown() = 0;

	virtual RHIContext *get_rhi_context() = 0;

	virtual PlatformWindow *create_window() = 0;
	virtual void destroy_window(PlatformWindow *window) = 0;

	virtual RHISurface *create_surface(PlatformWindow *window) = 0;
	virtual void destroy_surface(RHISurface *surface) = 0;
};