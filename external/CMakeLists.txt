include(FetchContent)

#######################################################################
# volk
#######################################################################
FetchContent_Declare(volk
    GIT_REPOSITORY https://github.com/zeux/volk.git
    GIT_TAG b465507a1720ea32d65163153a53ffa136ca835f#1.3.295
)
# Set a suitable platform define to compile volk with.
if(CMAKE_SYSTEM_NAME STREQUAL Windows)
  set(VOLK_STATIC_DEFINES VK_USE_PLATFORM_WIN32_KHR)
elseif(CMAKE_SYSTEM_NAME STREQUAL Linux)
  set(VOLK_STATIC_DEFINES VK_USE_PLATFORM_XLIB_KHR)
elseif(CMAKE_SYSTEM_NAME STREQUAL Darwin)
  set(VOLK_STATIC_DEFINES VK_USE_PLATFORM_MACOS_MVK)
endif()
FetchContent_MakeAvailable(volk)

#######################################################################
# glm
#######################################################################
FetchContent_Declare(
	glm
	#GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_REPOSITORY git@github.com:g-truc/glm.git
	GIT_TAG 0af55ccecd98d4e5a8d1fad7de25ba429d60e863#refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)