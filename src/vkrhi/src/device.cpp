#include "vkrhi/device.h"

#include "util.h"

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
