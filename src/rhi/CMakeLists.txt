add_library(rhi STATIC
    rhi_vulkan.h
    rhi_context_vulkan.h
    rhi_context_vulkan.cpp
)

target_include_directories(rhi PRIVATE
    ..
)

target_link_libraries(rhi PUBLIC
    volk::volk
    GPUOpen::VulkanMemoryAllocator
)