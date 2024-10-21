#include "rhi_context_vulkan.h"

#include <stdexcept>

const bool UseValidationLayers = true;

RHIContextVulkan::RHIContextVulkan(const std::vector<const char *> &extensions) {
	if (volkInitialize() != VK_SUCCESS) {
		throw std::runtime_error("volk init failed.");
	}
	init_system_info();
	check_version();
	init_extensions(extensions);
}

RHIContextVulkan::~RHIContextVulkan() {
	vkDestroyInstance(instance, nullptr);
}

uint32_t RHIContextVulkan::get_device_count() const {
	return 0;
}

const RHIContext::Device &RHIContextVulkan::get_device(uint32_t id) const {
	return Device{};
}

RHIDevice *RHIContextVulkan::create_device() {
	return nullptr;
}

const uint32_t RequiredVulkanVersion = VK_MAKE_API_VERSION(0, 1, 3, 0);
void RHIContextVulkan::check_version() {
	uint32_t version;
	if (!vkEnumerateInstanceVersion ||
			vkEnumerateInstanceVersion(&version) != VK_SUCCESS ||
			version < RequiredVulkanVersion) {
		throw std::runtime_error("vulkan version not available.");
	}
}

void RHIContextVulkan::init_extensions(const std::vector<const char *> platform_extensions) {
	for (auto e : platform_extensions) {
		add_extension(e);
	}
	if (UseValidationLayers) {
		add_extension(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}
}

void RHIContextVulkan::create_instance() {
	std::vector<const char *> extensions;
	extensions.reserve(enabled_extensions.size());
	for (auto e : enabled_extensions) {
		extensions.push_back(e.c_str());
	}
	std::vector<const char *> layers;
	if (UseValidationLayers) {
		layers.push_back("VK_LAYER_KHRONOS_validation");
	}

	VkApplicationInfo app_info = {};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pNext = nullptr;
	app_info.pApplicationName = "";
	app_info.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
	app_info.pEngineName = "";
	app_info.engineVersion = VK_MAKE_VERSION(0, 0, 1);
	app_info.apiVersion = RequiredVulkanVersion;
}

void RHIContextVulkan::init_system_info() {
	// layer properties.
	get_vector<VkLayerProperties>(available_layers, vkEnumerateInstanceLayerProperties);
	// extensions.
	get_vector<VkExtensionProperties>(available_extensions, vkEnumerateInstanceExtensionProperties, nullptr);
	// layer extensions.
	for (auto &layer : this->available_layers) {
		std::vector<VkExtensionProperties> layer_extensions;
		if (get_vector<VkExtensionProperties>(layer_extensions, vkEnumerateInstanceExtensionProperties, layer.layerName) != VK_SUCCESS) {
			available_extensions.insert(available_extensions.end(), layer_extensions.begin(), layer_extensions.end());
		}
	}
}

bool RHIContextVulkan::is_layer_available(const char *layer_name) const {
	for (const auto &layer_properties : available_layers) {
		if (strcmp(layer_name, layer_properties.layerName) == 0) {
			return true;
		}
	}
	return false;
}

bool RHIContextVulkan::is_extension_available(const char *extension_name) const {
	for (const auto &extension_properties : available_extensions) {
		if (strcmp(extension_name, extension_properties.extensionName) == 0) {
			return true;
		}
	}
	return false;
}

void RHIContextVulkan::add_extension(const char *extension_name) {
	if (!is_extension_available(extension_name)) {
		throw std::runtime_error(std::string("missing vulkan extension:") + extension_name);
	}
	enabled_extensions.insert(extension_name);
}