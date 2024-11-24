#pragma once

#include "vkrhi/common.h"

#include "vkrhi/queue.h"

namespace vkrhi {

class Device {
public:
	Device() = delete;
	Device(const Device &) = delete;
	Device(const vkb::Instance &instance, const vkb::PhysicalDevice &physical_dev);
	~Device();

	const vkb::DispatchTable &get_api() const { return api; }
	VmaAllocator get_vma_allocator() const { return vma_allocator; }

private:
	vkb::Instance instance;
	vkb::PhysicalDevice physical_device;
	vkb::Device device;
	vkb::DispatchTable api;
	VmaAllocator vma_allocator;
	std::vector<std::vector<Queue>> queues;
};

} // namespace vkrhi
