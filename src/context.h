#pragma once

#include "VkBootstrap.h"

class Context
{
public:
    Context();
    Context(const Context &) = delete;
    void operator=(const Context &) = delete;
    ~Context();

    const vkb::Instance &instance() const { return _instance; }
    const vkb::InstanceDispatchTable &table() const { return _instanceTable; }

private:
    vkb::Instance _instance;
    vkb::InstanceDispatchTable _instanceTable;
};