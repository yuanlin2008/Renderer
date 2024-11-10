#pragma once

#include "SDL3/SDL.h"
#include "rhi_context.h"
#include <cstdint>

struct RHICommandQueue;
struct RHICommandPool {};
struct RHICommandBuffer {};
struct RHIBuffer {};
struct RHITexture {};
struct RHISampler {};
struct RHIVertexFormat {};
struct RHISwapChain;
struct RHIFramebuffer {};
struct RHIShader {};
struct RHIUniformSet {};
struct RHIPipeline {};
struct RHIRenderPass {};
struct RHIQueryPool {};
struct RHIFence;

enum class RHIMemoryType {
	DeviceLocal,
	Upload,
	ReadBack,
};

enum class RHIFormat {
	// D3D formats omitted: 19-22, 44-47, 65-66, 68-70, 73, 76, 79, 82, 88-89, 92-94, 97, 100-114
	// These formats are omitted due to lack of a corresponding Vulkan format. D24_UNORM_S8_UINT
	// (DXGI_FORMAT 45) has a matching Vulkan format but is also omitted as it is only supported by
	// Nvidia.
	Unknown,

	R32G32B32A32_TYPELESS,
	R32G32B32_TYPELESS,
	R32G32_TYPELESS,
	R32_TYPELESS,

	R16G16B16A16_TYPELESS,
	R16G16_TYPELESS,
	R16_TYPELESS,

	R8G8B8A8_TYPELESS,
	R8G8_TYPELESS,
	R8_TYPELESS,
	B8G8R8A8_TYPELESS,

	R32G32B32A32_FLOAT,
	R32G32B32_FLOAT,
	R32G32_FLOAT,
	R32_FLOAT,

	R16G16B16A16_FLOAT,
	R16G16_FLOAT,
	R16_FLOAT,

	R32G32B32A32_UINT,
	R32G32B32_UINT,
	R32G32_UINT,
	R32_UINT,

	R16G16B16A16_UINT,
	R16G16_UINT,
	R16_UINT,

	R8G8B8A8_UINT,
	R8G8_UINT,
	R8_UINT,

	R32G32B32A32_SINT,
	R32G32B32_SINT,
	R32G32_SINT,
	R32_SINT,

	R16G16B16A16_SINT,
	R16G16_SINT,
	R16_SINT,

	R8G8B8A8_SINT,
	R8G8_SINT,
	R8_SINT,

	R16G16B16A16_UNORM,
	R16G16_UNORM,
	R16_UNORM,

	R8G8B8A8_UNORM,
	R8G8B8A8_UNORM_SRGB,
	R8G8_UNORM,
	R8_UNORM,
	B8G8R8A8_UNORM,
	B8G8R8A8_UNORM_SRGB,
	B8G8R8X8_UNORM,
	B8G8R8X8_UNORM_SRGB,

	R16G16B16A16_SNORM,
	R16G16_SNORM,
	R16_SNORM,

	R8G8B8A8_SNORM,
	R8G8_SNORM,
	R8_SNORM,

	D32_FLOAT,
	D16_UNORM,
	D32_FLOAT_S8_UINT,
	R32_FLOAT_X32_TYPELESS,

	B4G4R4A4_UNORM,
	B5G6R5_UNORM,
	B5G5R5A1_UNORM,

	R9G9B9E5_SHAREDEXP,
	R10G10B10A2_TYPELESS,
	R10G10B10A2_UNORM,
	R10G10B10A2_UINT,
	R11G11B10_FLOAT,

	BC1_UNORM,
	BC1_UNORM_SRGB,
	BC2_UNORM,
	BC2_UNORM_SRGB,
	BC3_UNORM,
	BC3_UNORM_SRGB,
	BC4_UNORM,
	BC4_SNORM,
	BC5_UNORM,
	BC5_SNORM,
	BC6H_UF16,
	BC6H_SF16,
	BC7_UNORM,
	BC7_UNORM_SRGB,

	R64_UINT,

	R64_SINT,

	_Count,
};

enum class RHICommandQueueType {
	Graphics,
	Compute,
	Transfer,
};

class RHIDevice {
public:
	virtual ~RHIDevice() {}

	/******************************
	 * Command Queue.
	 ******************************/
	virtual RHICommandQueue *get_command_queue(RHICommandQueueType type) = 0;

	/******************************
	 * Command Pool.
	 ******************************/
	// virtual RHICommandPool *create_command_pool(RHICommandQueueType type) = 0;
	// virtual void destroy_command_pool(RHICommandPool *pool) = 0;
	// virtual void reset_command_pool(RHICommandPool* pool) = 0;

	/******************************
	 * Swapchain.
	 ******************************/
	virtual RHISwapChain *create_swapchain(SDL_Window *window, RHICommandQueue *queue, RHIFormat format, uint32_t count) = 0;
	virtual void destroy_swapchain(RHISwapChain *swapchain) = 0;
	// virtual void resize_swapchain(RHISwapChain *swapchain) = 0;

	/******************************
	 * Buffer
	 ******************************/
	// virtual RHIBuffer *create_buffer(uint64_t size, uint32_t usage, RHIMemoryType alloc_type) = 0;
	// virtual void destroy_buffer(RHIBuffer *buffer) = 0;
	// virtual uint8_t *map_buffer_memory(RHIBuffer *buffer) = 0;
	// virtual void unmap_buffer_memory(RHIBuffer *buffer) = 0;

	/******************************
	 * Command Pool.
	 ******************************/
	//virtual RHICommandBuffer *alloc_command_buffer(RHICommandPool *pool) = 0;
	//virtual void begin_command_buffer(RHICommandBuffer *cmd_buffer) = 0;
	//virtual void end_command_buffer(RHICommandBuffer *cmd_buffer) = 0;

	/******************************
	 * Fence
	 ******************************/
	virtual RHIFence *create_fence(uint64_t init_value) = 0;
	virtual void destroy_fence(RHIFence *fence) = 0;
	virtual uint64_t get_fence_value(RHIFence *fence) = 0;
	virtual void wait_fences(RHIFence **fence, uint64_t *value, uint32_t count) = 0;
	virtual void signal_fence(RHIFence *fence, uint64_t value) = 0;
};