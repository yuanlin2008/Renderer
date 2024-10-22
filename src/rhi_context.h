#pragma once

#include <string>
#include <vector>

struct RHISurface {};

class RHIContext {
public:
	virtual class RHIDevice *create_device(struct RHISurface *surface) = 0;
};