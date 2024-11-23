#pragma once

#include "vkrhi/common.h"

namespace vkrhi {

class Device {
public:
	Device() = delete;
	Device(const Device &) = delete;
	Device(const vkb::Instance &instance, const vkb::PhysicalDevice &physical_dev);
	~Device();

private:
	vkb::Instance instance;
	vkb::PhysicalDevice physical_device;
	vkb::Device device;
	vkb::DispatchTable api;
	VmaAllocator vma_allocator;
};

} // namespace vkrhi
