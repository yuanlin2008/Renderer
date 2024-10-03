#include <string>
#include <stdexcept>
#include "rhi.h"
#include "spdlog/spdlog.h"

void RHI::init()
{
    //
    // create instance.
    //
    vkb::InstanceBuilder instanceBuilder;
    auto instRet = instanceBuilder
                       .set_app_name("Renderer")
                       .request_validation_layers(true)
                       .use_default_debug_messenger()
                       .require_api_version(1, 3, 225)
                       .build();
    if (!instRet)
    {
        throw std::runtime_error(
            std::string("Failed to create Vulkan instance. Error: ") +
            instRet.error().message());
    }
    _instance = instRet.value();

    //
    // select a gpu.
    //

    // vulkan 1.3 features
    VkPhysicalDeviceVulkan13Features features13{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
        .synchronization2 = true,
        .dynamicRendering = true,
    };

    // vulkan 1.2 features
    VkPhysicalDeviceVulkan12Features features12{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
        .descriptorIndexing = true,
        .bufferDeviceAddress = true,
    };

    vkb::PhysicalDeviceSelector pdSelector(_instance);
    vkb::PhysicalDevice phyDevice = pdSelector
                                        .set_minimum_version(1, 3)
                                        .set_required_features_13(features13)
                                        .set_required_features_12(features12)
                                        .defer_surface_initialization()
                                        .select()
                                        .value();
    SPDLOG_INFO("Physical Device: {}", phyDevice.name);

    //
    // create device.
    //
    vkb::DeviceBuilder deviceBuilder(phyDevice);
    _device = deviceBuilder.build().value();
    _deviceFN = _device.make_table();
    _graphicQueue = _device.get_queue(vkb::QueueType::graphics).value();
}

void RHI::close()
{
    _deviceFN.deviceWaitIdle();
    vkb::destroy_device(_device);
    vkb::destroy_instance(_instance);
}