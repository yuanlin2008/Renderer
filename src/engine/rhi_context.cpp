#include "rhi_context.h"

#include <cassert>

static RHIContext *instance = nullptr;

RHIContext::RHIContext() {
	assert(!instance);
	instance = this;
}

RHIContext::~RHIContext() {
	assert(instance);
	instance = nullptr;
}

RHIContext *RHIContext::get_instance() {
	return instance;
}