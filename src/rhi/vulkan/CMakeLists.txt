add_library(rhi_vulkan STATIC
    rhi_context_vulkan.h rhi_context_vulkan.cpp
    rhi_device_vulkan.h rhi_device_vulkan.cpp 
    vk_util.h vk_util.cpp
)

target_include_directories(rhi_vulkan 
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(rhi_vulkan PUBLIC
    vk-bootstrap::vk-bootstrap
    GPUOpen::VulkanMemoryAllocator
    engine
)