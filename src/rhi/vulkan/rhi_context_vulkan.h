#pragma once

#include "vk_util.h"

class RHIContextVulkan : public RHIContext {
public:
	RHIContextVulkan();
	~RHIContextVulkan();

	virtual RHIDevice *create_device() override;
	virtual void destroy_device(RHIDevice *device) override;

	vkb::Instance instance;
	vkb::InstanceDispatchTable api;
};