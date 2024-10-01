#pragma once

#include "volk.h"
#include <vector>

class RHI
{
public:
    void init();
    void close();

private:
    VkInstance instance_ = VK_NULL_HANDLE;
    std::vector<VkExtensionProperties> extensions_;
};