#pragma once

#include "vkrhi/common.h"

namespace vkrhi {

class Device;

class Queue : public Object<VkQueue> {
public:
	Queue(Device &device, uint32_t family_index, uint32_t index);

	uint32_t get_family_index() const { return family_index; }
	uint32_t get_index() const { return index; }

	VkResult submit(const std::vector<VkSubmitInfo> &submit_infos, VkFence fence) const;
	VkResult present(const VkPresentInfoKHR &present_info) const;
	VkResult wait_idle() const;

private:
	uint32_t family_index{ 0 };
	uint32_t index{ 0 };
};

} // namespace vkrhi
