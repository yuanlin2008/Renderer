#include "rhi.h"

void RHI::init()
{
    vkb::InstanceBuilder instanceBuilder;
    auto instRet = instanceBuilder
                       .set_app_name("Renderer")
                       .request_validation_layers(false)
                       .use_default_debug_messenger()
                       .require_api_version(1, 3, 225)
                       .build();
}

void RHI::close()
{
}