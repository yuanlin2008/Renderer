#pragma once

#include "rhi_device.h"

class RHIContextVulkan;

class RHIDeviceVulkan : public RHIDevice {
public:
	RHIDeviceVulkan(RHIContextVulkan *context);
};