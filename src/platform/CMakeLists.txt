add_executable(renderer 
    platform_sdl.cpp
    platform_sdl.h
    main.cpp
)

target_link_libraries(renderer PRIVATE
    spdlog::spdlog_header_only
    SDL2::SDL2-static 
    engine
    rhi
)