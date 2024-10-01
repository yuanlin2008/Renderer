project(Renderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Renderer 
    main.cpp
    rhi.h rhi.cpp
)
target_link_libraries(Renderer volk glfw)