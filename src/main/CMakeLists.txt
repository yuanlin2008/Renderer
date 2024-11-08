add_executable(renderer 
    platform_sdl.cpp
    platform_sdl.h
    main.cpp
)

target_link_libraries(renderer PRIVATE
    engine
    rhi
)