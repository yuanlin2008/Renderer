#pragma once

#include "VkBootstrap.h"

#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1
#include "vk_mem_alloc.h"

namespace vkrhi {

class Device;

template <typename HandleType>
class Object {
public:
	Object(Device &device) :
			device(device) {}
	operator HandleType() const { return handle; }

protected:
	Device &device;
	HandleType handle{ VK_NULL_HANDLE };
};

} // namespace vkrhi