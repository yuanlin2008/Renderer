#pragma once

#include <string>
#include <vector>

struct RHISurface {};

class RHIContext {
public:
	virtual ~RHIContext() {}
	virtual class RHIDevice *create_device(RHISurface *surface) = 0;
};