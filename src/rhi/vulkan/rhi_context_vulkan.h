#pragma once

#include "vk_util.h"

class RHIContextVulkan : public RHIContext {
public:
	RHIContextVulkan();
	~RHIContextVulkan();

	virtual RHIDevice *create_device() override;
	virtual void destroy_device(RHIDevice *device) override;

	vkb::Instance &get_instance() { return instance; }
	vkb::InstanceDispatchTable &get_instance_funcs() { return instance_funcs; }

private:
	vkb::Instance instance;
	vkb::InstanceDispatchTable instance_funcs;
};