#pragma once

#include "VkBootstrap.h"
#include "rhi_context.h"

#define VK_CHECK(f)                                                                \
	{                                                                              \
		VkResult res = (f);                                                        \
		if (res != VK_SUCCESS) {                                                   \
			throw std::runtime_error(std::string(#f) + ":" + get_result_str(res)); \
		}                                                                          \
	}

std::string get_result_str(VkResult errorCode);
VkFormat get_format(RHIFormat format);