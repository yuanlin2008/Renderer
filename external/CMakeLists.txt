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
# vma
FetchContent_Declare(
	vma
	#GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git
	GIT_REPOSITORY git@github.com:GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git
	GIT_TAG 009ecd192c1289c7529bff248a16cfe896254816 # 3.1.0
)
FetchContent_MakeAvailable(vma)

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

###############################################################
# imgui
FetchContent_Declare(
	imgui
	#GIT_REPOSITORY https://github.com/ocornut/imgui.git
	GIT_REPOSITORY git@github.com:ocornut/imgui.git
	GIT_TAG cb16568fca5297512ff6a8f3b877f461c4323fbe # 1.91.3
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
)
FetchContent_MakeAvailable(imgui)
# todo imgui 

###############################################################
# stb
FetchContent_Declare(
	stb
	#GIT_REPOSITORY https://github.com/nothings/stb.git
	GIT_REPOSITORY git@github.com:nothings/stb.git
	GIT_TAG 31707d14fdb75da66b3eed52a2236a70af0d0960
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
)
FetchContent_MakeAvailable(stb)
# todo stb 