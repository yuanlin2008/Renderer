add_executable(renderer 
    main.cpp
)

target_link_libraries(renderer PRIVATE
    engine
    rhi_vulkan
)