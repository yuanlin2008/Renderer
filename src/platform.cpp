#include "platform.h"

Platform *instance = nullptr;

Platform::Platform() {
	instance = this;
}

Platform::~Platform() {
	instance = nullptr;
}

Platform *Platform::get_instance() {
	return instance;
}