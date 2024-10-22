#pragma once

#include "rhi_context.h"

#include "VkBootstrap.h"

struct RHISurfaceVulkan : public RHISurface {
	VkSurfaceKHR surface;
};

class RHIContextVulkan : public RHIContext {
public:
	RHIContextVulkan(const std::vector<const char *> &platform_extensions);
	~RHIContextVulkan();

	virtual class RHIDevice *create_device(RHISurface *surface) override;

	vkb::Instance get_instance() { return instance; }

private:
	vkb::Instance instance;
};