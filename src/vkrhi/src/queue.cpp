#include "vkrhi/queue.h"

namespace vkrhi {

Queue::Queue(Device &device, uint32_t family_index, uint32_t index) :
		Object{ device },
		family_index{ family_index },
		index{ index } {
}

VkResult Queue::submit(const std::vector<VkSubmitInfo> &submit_infos, VkFence fence) const {
}
VkResult Queue::present(const VkPresentInfoKHR &present_info) const {
}
VkResult Queue::wait_idle() const {
}

} // namespace vkrhi
