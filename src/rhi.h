#pragma once

#include "volk.h"

class RHI
{
public:
    void init();
    void close();
private:
    VkInstance instance_ = VK_NULL_HANDLE;
};