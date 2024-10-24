#pragma once

#include "rhi_device.h"

#include <string>
#include <vector>

struct RHISurface {};

class RHIContext {
public:
	virtual ~RHIContext() {}
	virtual RHIDevice *create_device(RHISurface *surface) = 0;
	virtual void destroy_device(RHIDevice *device) = 0;
};