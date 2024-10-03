#pragma once

#include "VkBootstrap.h"

class RHI
{
public:
    void init();
    void close();

private:
    vkb::Instance _instance;
};