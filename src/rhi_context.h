#pragma once

#include <string>
#include <vector>

class RHIContext {
public:
	virtual class RHIDevice *create_device() = 0;
};