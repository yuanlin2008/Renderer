cmake_minimum_required(VERSION 3.30)

if(MSVC)
    add_compile_options(/wd"4819")
endif()

add_subdirectory(external)
add_subdirectory(src)