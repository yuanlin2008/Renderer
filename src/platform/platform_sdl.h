#pragma once

#include "platform.h"

class PlatformSDL : public Platform {
	virtual void initialize() override;
	virtual void shutdown() override;
	virtual void run() override;
};