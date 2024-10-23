#pragma once

class Engine {
public:
	static void initialize();
	static bool iteration();
	static void shutdown();

	static void quit();
};