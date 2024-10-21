add_library(rhi STATIC
    rhi_context_vulkan.h
    rhi_context_vulkan.cpp
)

target_include_directories(rhi PRIVATE
    ..
)

target_link_libraries(rhi PRIVATE
    vk-bootstrap::vk-bootstrap
    GPUOpen::VulkanMemoryAllocator
)