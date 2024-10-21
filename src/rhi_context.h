#pragma once

#include <string>
#include <vector>

class RHIContext {
public:
	enum DeviceType {
		INTEGRATED,
		DISCRETE,
		OTHER
	};
	struct Device {
		std::string name;
		DeviceType type = DeviceType::OTHER;
	};
	virtual uint32_t get_device_count() const = 0;
	virtual const Device &get_device(uint32_t id) const = 0;
	virtual class RHIDevice *create_device() = 0;
};