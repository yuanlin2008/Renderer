#pragma once

#include "VkBootstrap.h"

#define VK_CHECK_RESULT(f)                                                       \
	{                                                                            \
		VkResult res = (f);                                                      \
		if (res != VK_SUCCESS) {                                                 \
			throw std::runtime_error(std::string(#f) + ":" + vk_error_str(res)); \
		}                                                                        \
	}

std::string vk_error_str(VkResult errorCode);