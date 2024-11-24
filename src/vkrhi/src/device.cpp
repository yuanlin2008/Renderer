#include "vkrhi/device.h"

namespace vkrhi {

Device::Device(const vkb::Instance &instance, const vkb::PhysicalDevice &physical_dev) :
		instance(instance),
		physical_device(physical_dev) {
	// create device.
	vkb::DeviceBuilder builder(physical_device);
	auto ret = builder.build();
	if (!ret) {
		throw std::runtime_error(
				std::string("Failed to create device. Error: ") +
				ret.error().message());
	}
	device = ret.value();
	api = device.make_table();

	// create queues.
	queues.resize(device.queue_families.size());
	for (uint32_t queue_familiy_index = 0; queue_familiy_index < device.queue_families.size(); ++queue_familiy_index) {
		const auto &queue_family = device.queue_families[queue_familiy_index];
		for (uint32_t index = 0; index < queue_family.queueCount; ++index) {
			queues[queue_familiy_index].emplace_back(*this, queue_familiy_index, queue_family, index);
		}
	}

	// create vma.
	VmaVulkanFunctions vma_funcs = {
		.vkGetInstanceProcAddr = instance.fp_vkGetInstanceProcAddr,
		.vkGetDeviceProcAddr = instance.fp_vkGetDeviceProcAddr
	};
	VmaAllocatorCreateInfo alloc_info = {
		.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
		.physicalDevice = physical_device,
		.device = device,
		.pVulkanFunctions = &vma_funcs,
		.instance = instance
	};
	VK_CHECK(vmaCreateAllocator(&alloc_info, &vma_allocator));
}

Device::~Device() {
	vmaDestroyAllocator(vma_allocator);
	vkb::destroy_device(device);
}

} // namespace vkrhi
