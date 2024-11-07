#include "rhi_context_vulkan.h"

#include "rhi_device_vulkan.h"
#include "spdlog/spdlog.h"

const bool UseValidationLayers = true;

RHIContextVulkan::RHIContextVulkan() {
	vkb::InstanceBuilder builder;
	auto ret = builder.set_app_name("Renderer")
					   .request_validation_layers(UseValidationLayers)
					   .use_default_debug_messenger()
					   .require_api_version(1, 3, 0)
					   .build();
	if (!ret) {
		throw std::runtime_error(
				std::string("Failed to create Vulkan instance. Error: ") +
				ret.error().message());
	}
	instance = ret.value();
	instance_funcs = instance.make_table();
}

RHIContextVulkan::~RHIContextVulkan() {
	vkb::destroy_instance(instance);
}

RHIDevice *RHIContextVulkan::create_device(RHISurface *surface) {
	// select a device.
	VkPhysicalDeviceVulkan13Features features13{
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
		.synchronization2 = true,
		.dynamicRendering = true,
	};

	VkPhysicalDeviceVulkan12Features features12{
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
		.descriptorIndexing = true,
		.bufferDeviceAddress = true,
	};

	vkb::PhysicalDeviceSelector selector(instance);
	auto ret = selector.set_minimum_version(1, 3)
					   .set_required_features_13(features13)
					   .set_required_features_12(features12)
					   .set_surface(static_cast<RHISurfaceVulkan *>(surface)->surface)
					   .select();
	if (!ret) {
		throw std::runtime_error(
				std::string("Failed to select device. Error: ") +
				ret.error().message());
	}

	vkb::PhysicalDevice device = ret.value();
	SPDLOG_INFO("Physical Device: {}", device.name);
	return new RHIDeviceVulkan(this, device);
}

void RHIContextVulkan::destroy_device(RHIDevice *device) {
	delete device;
}