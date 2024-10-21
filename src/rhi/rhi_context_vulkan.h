#pragma once

#include "rhi_context.h"

#include "rhi_vulkan.h"

#include <set>
#include <string>
#include <vector>

class RHIContextVulkan : public RHIContext {
public:
	RHIContextVulkan(const std::vector<const char *> &platform_extensions);
	~RHIContextVulkan();

	virtual uint32_t get_device_count() const override;
	virtual const Device &get_device(uint32_t id) const override;
	virtual class RHIDevice *create_device() override;

	VkInstance get_nstance() { return instance; }

private:
	void init_system_info();
	void check_version();
	void init_extensions(const std::vector<const char *> platform_extensions);
	void create_instance();

	bool is_layer_available(const char *layer_name) const;
	bool is_extension_available(const char *extension_name) const;
	void add_extension(const char *extension_name);

	std::vector<VkLayerProperties> available_layers;
	std::vector<VkExtensionProperties> available_extensions;
	VkInstance instance;
	std::set<std::string> enabled_extensions;
};