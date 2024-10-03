include(FetchContent)

###############################################################
# SDL2
FetchContent_Declare(
	SDL
	#GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_REPOSITORY git@github.com:libsdl-org/SDL.git
	GIT_TAG 79ec168f3c1e2fe27335cb8886439f7ef676fb49 #2.30.8
)
FetchContent_MakeAvailable(SDL)

###############################################################
# vk-bootstrap
FetchContent_Declare(
	vk-bootstrap
	#GIT_REPOSITORY https://github.com/charles-lunarg/vk-bootstrap.git
  GIT_REPOSITORY git@github.com:charles-lunarg/vk-bootstrap.git
	GIT_TAG 5836b6cb94bb42102757d13e0cdeacda9df5f0d4 #1.3.290
)
FetchContent_MakeAvailable(vk-bootstrap)

###############################################################
# volk
#[[
FetchContent_Declare(volk
    #GIT_REPOSITORY https://github.com/zeux/volk.git
    GIT_REPOSITORY git@github.com:zeux/volk.git
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
]]

###############################################################
# glm
FetchContent_Declare(
	glm
	#GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_REPOSITORY git@github.com:g-truc/glm.git
	GIT_TAG 0af55ccecd98d4e5a8d1fad7de25ba429d60e863#refs/tags/1.0.1
)
FetchContent_MakeAvailable(glm)

###############################################################
# spdlog
FetchContent_Declare(
	spdlog
	#GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_REPOSITORY git@github.com:gabime/spdlog.git
	GIT_TAG 27cb4c76708608465c413f6d0e6b8d99a4d84302 #1.14.1
)
FetchContent_MakeAvailable(spdlog)