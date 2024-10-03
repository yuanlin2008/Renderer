project(Renderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Renderer WIN32
    main.cpp
    rhi.h rhi.cpp
)
target_link_libraries(Renderer PRIVATE 
    vk-bootstrap::vk-bootstrap
    SDL2::SDL2-static 
    SDL2::SDL2main)