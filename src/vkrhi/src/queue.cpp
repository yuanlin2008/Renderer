#include "vkrhi/queue.h"

#include "vkrhi/device.h"

namespace vkrhi {

Queue::Queue(Device &device, uint32_t family_index, VkQueueFamilyProperties properties, uint32_t index) :
		Object{ device },
		family_index{ family_index },
		properties{ properties },
		index{ index } {
	device.get_api().getDeviceQueue(family_index, index, &handle);
}

VkResult Queue::submit(const std::vector<VkSubmitInfo> &submit_infos, VkFence fence) const {
	return device.get_api().queueSubmit(*this, static_cast<uint32_t>(submit_infos.size()), submit_infos.data(), fence);
}

VkResult Queue::present(const VkPresentInfoKHR &present_info) const {
	return device.get_api().queuePresentKHR(*this, &present_info);
}

VkResult Queue::wait_idle() const {
	return device.get_api().queueWaitIdle(*this);
}

} // namespace vkrhi
