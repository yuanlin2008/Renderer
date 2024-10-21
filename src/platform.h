#pragma once

class RHIContext;

class Platform {
public:
	Platform();
	~Platform();
	static Platform *get_instance();

	virtual void initialize() = 0;
	virtual void shutdown() = 0;
	virtual void run() = 0;

	RHIContext *get_rhi_context() { return rhi_context; }

protected:
	RHIContext *rhi_context = nullptr;
};