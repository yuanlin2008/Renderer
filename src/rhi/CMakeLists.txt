add_library(rhi STATIC
    rhi_context_vulkan.h 
    rhi_context_vulkan.cpp
    rhi_device_vulkan.h 
    rhi_device_vulkan.cpp 
)

target_include_directories(rhi 
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(rhi PUBLIC
    spdlog::spdlog_header_only
    #volk::volk
    vk-bootstrap::vk-bootstrap
    GPUOpen::VulkanMemoryAllocator
    engine
)