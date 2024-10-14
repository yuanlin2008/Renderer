#include "context.h"

Context::Context()
{
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
    _instanceTable = _instance.make_table();

}
Context::~Context()
{
    vkb::destroy_instance(_instance);
}
