#pragma once

#include "rhi_context.h"

#include "VkBootstrap.h"

class RHIContextVulkan : public RHIContext {
public:
	RHIContextVulkan(const std::vector<const char *> &platform_extensions);
	~RHIContextVulkan();

	virtual class RHIDevice *create_device() override;

	vkb::Instance get_instance() { return instance; }

private:
	vkb::Instance instance;
};