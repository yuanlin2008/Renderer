#pragma once

#include "rhi_context.h"

class RHIDevice {
public:
	virtual ~RHIDevice() {}

	enum class CommandQueueType {
		Graphics,
		Compute,
		Transfer,
	};
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

	virtual CommandQueue *get_command_queue(CommandQueueType type) = 0;

	virtual CommandPool *create_command_pool(CommandQueueType type) = 0;
	virtual void destroy_command_pool(CommandPool *pool) = 0;

	virtual CommandBuffer *alloc_command_buffer(CommandPool *pool) = 0;
	virtual void begin_command_buffer(CommandBuffer *cmd_buffer) = 0;
	virtual void end_command_buffer(CommandBuffer *cmd_buffer) = 0;

	virtual Fence *create_fence() = 0;
};