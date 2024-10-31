#pragma once

#include "rhi_context.h"

class RHIDevice {
public:
	virtual ~RHIDevice() {}

	/******************************
	 * RHI Objects.
	 ******************************/
	struct CommandQueue {};
	struct CommandPool {};
	struct CommandBuffer {};
	struct Buffer {};
	struct Texture {};
	struct Sampler {};
	struct VertexFormat {};
	struct SwapChain {};
	struct Framebuffer {};
	struct Shader {};
	struct UniformSet {};
	struct Pipeline {};
	struct RenderPass {};
	struct QueryPool {};
	struct Fence {};
	struct Semaphore {};

	/******************************
	 * Memory
	 ******************************/
	enum class MemoryAllocationType {
		CPU, // For images, CPU allocation also means linear, GPU is tiling optimal.
		GPU,
	};

	/******************************
	 * Buffer
	 ******************************/
	enum class BufferUsage {
		TRANSFER_SRC = (1 << 0),
		TRANSFER_DST = (1 << 1),
		UNIFORM_TEXEL = (1 << 2),
		UNIFORM_BUFFER = (1 << 4),
		STORAGE_BUFFER = (1 << 5),
		INDEX_BUFFER = (1 << 6),
		VERTEX_BUFFER = (1 << 7),
		INDIRECT_BUFFER = (1 << 8),
	};

	/******************************
	 * Command Queue.
	 ******************************/
	enum class CommandQueueType {
		Graphics,
		Compute,
		Transfer,
	};

	virtual CommandQueue *get_command_queue(CommandQueueType type) = 0;

	/******************************
	 * Command Pool.
	 ******************************/
	virtual CommandPool *create_command_pool(CommandQueueType type) = 0;
	virtual void destroy_command_pool(CommandPool *pool) = 0;

	/******************************
	 * Command Pool.
	 ******************************/
	virtual CommandBuffer *alloc_command_buffer(CommandPool *pool) = 0;
	virtual void begin_command_buffer(CommandBuffer *cmd_buffer) = 0;
	virtual void end_command_buffer(CommandBuffer *cmd_buffer) = 0;

	virtual Fence *create_fence() = 0;
};