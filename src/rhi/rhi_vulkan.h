#pragma once

#include "VkBootstrap.h"
#include "rhi_context.h"

#define VK_CHECK_RESULT(f)                                                                 \
	{                                                                                      \
		VkResult res = (f);                                                                \
		if (res != VK_SUCCESS) {                                                           \
			throw std::runtime_error(std::string(#f) + ":" + Vulkan::get_result_str(res)); \
		}                                                                                  \
	}

class Vulkan {
public:
	static std::string get_result_str(VkResult errorCode);
	static VkFormat get_format(RHIFormat format);
};