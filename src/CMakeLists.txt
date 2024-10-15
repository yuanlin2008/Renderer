project(Renderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Renderer 
    main.cpp
    context.h context.cpp
    device.h device.cpp
    command_buffer.h command_buffer.cpp
    renderer.h renderer.cpp
)
target_link_libraries(Renderer PRIVATE 
    spdlog::spdlog_header_only
    vk-bootstrap::vk-bootstrap
    SDL2::SDL2-static 
    GPUOpen::VulkanMemoryAllocator
    )