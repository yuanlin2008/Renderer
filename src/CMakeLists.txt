project(Renderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Renderer main.cpp)
target_link_libraries(Renderer volk glfw)