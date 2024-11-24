add_library(vkrhi STATIC
    include/vkrhi/common.h
    include/vkrhi/device.h
    include/vkrhi/queue.h

    src/common.cpp
    src/device.cpp
    src/queue.cpp
)

target_include_directories(vkrhi 
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(vkrhi PUBLIC
    vk-bootstrap::vk-bootstrap
    GPUOpen::VulkanMemoryAllocator
    spdlog::spdlog_header_only
)