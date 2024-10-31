#pragma once

#include "rhi_context.h"
#include "rhi_vulkan.h"

struct RHISurfaceVulkan : public RHISurface {
	VkSurfaceKHR surface;
};

class RHIContextVulkan : public RHIContext {
public:
	RHIContextVulkan(const std::vector<const char *> &platform_extensions);
	~RHIContextVulkan();

	virtual RHIDevice *create_device(RHISurface *surface) override;
	virtual void destroy_device(RHIDevice *device) override;

	vkb::Instance &get_instance() { return instance; }
	vkb::InstanceDispatchTable &get_instance_funcs() { return instance_funcs; }

private:
	vkb::Instance instance;
	vkb::InstanceDispatchTable instance_funcs;
};