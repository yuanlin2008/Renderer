#include "rhi_context_vulkan.h"

#include "rhi_device_vulkan.h"
#include "spdlog/spdlog.h"

#include <stdexcept>

const bool UseValidationLayers = true;

RHIContextVulkan::RHIContextVulkan(const std::vector<const char *> &extensions) {
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
}

RHIContextVulkan::~RHIContextVulkan() {
	vkb::destroy_instance(instance);
}

RHIDevice *RHIContextVulkan::create_device(RHISurface *surface) {
	// select a gpu.
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

	vkb::PhysicalDeviceSelector pdSelector(instance);
	vkb::PhysicalDevice phyDevice = pdSelector.set_minimum_version(1, 3)
											.set_required_features_13(features13)
											.set_required_features_12(features12)
											.set_surface(static_cast<RHISurfaceVulkan *>(surface)->surface)
											.select()
											.value();
	SPDLOG_INFO("Physical Device: {}", phyDevice.name);
	return new RHIDeviceVulkan(this);
}
