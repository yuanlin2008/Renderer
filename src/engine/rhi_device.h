#pragma once

#include "rhi_context.h"

struct RHIBuffer {};
struct RHITexture {};
struct RHISampler {};
struct RHIVertexFormat {};
struct RHICommandQueue {};
struct RHICommandQueueFamily {};
struct RHICommandPool {};
struct RHICommandBuffer {};
struct RHISwapChain {};
struct RHIFramebuffer {};
struct RHIShader {};
struct RHIUniformSet {};
struct RHIPipeline {};
struct RHIRenderPass {};
struct RHIQueryPool {};
struct RHIFence {};
struct RHISemaphore {};

class RHIDevice {
public:
	virtual ~RHIDevice() {}
};