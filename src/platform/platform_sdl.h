#pragma once

#include "platform.h"

class PlatformSDL : public Platform {
public:
	PlatformSDL();
	~PlatformSDL();
	void handle_events();

	virtual RHIContext *get_rhi_context() override;
	virtual PlatformWindow *create_window(const char *title, int width, int height) override;
	virtual void destroy_window(PlatformWindow *window) override;
	virtual RHISurface *create_surface(PlatformWindow *window) override;
	virtual void destroy_surface(RHISurface *surface) override;

private:
	RHIContext *rhi_context = nullptr;
};