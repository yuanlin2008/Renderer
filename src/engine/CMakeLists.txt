add_library(engine STATIC
    rhi_context.h rhi_context.cpp
    rhi_device.h
)

target_include_directories(engine 
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(engine 
PUBLIC
    spdlog::spdlog_header_only
    SDL3::SDL3
)