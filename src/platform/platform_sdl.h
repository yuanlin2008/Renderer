#pragma once

#include "platform.h"

class PlatformSDL : public Platform {
public:
	virtual void initialize() override;
	virtual void shutdown() override;
	virtual void run() override;
	virtual RHIContext *get_rhi_context() override;
	virtual PlatformWindow *create_window() override;
	virtual void destroy_window(PlatformWindow *window) override;
	virtual RHISurface *create_surface(PlatformWindow *window) override;
	virtual void destroy_surface(RHISurface *surface) override;

private:
};