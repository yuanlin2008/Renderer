add_library(rhi STATIC
    rhi_context_vulkan.h rhi_context_vulkan.cpp
    rhi_device_vulkan.h rhi_device_vulkan.cpp 
)

target_include_directories(rhi PRIVATE
    ..
)

target_link_libraries(rhi PUBLIC
    #volk::volk
    vk-bootstrap::vk-bootstrap
    GPUOpen::VulkanMemoryAllocator
    spdlog::spdlog_header_only
)