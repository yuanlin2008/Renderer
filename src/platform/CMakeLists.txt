add_library(platform STATIC
    platform_sdl.cpp
    platform_sdl.h
)

target_include_directories(platform PRIVATE
    ..
)

target_link_libraries(platform PRIVATE
    SDL2::SDL2-static 
)