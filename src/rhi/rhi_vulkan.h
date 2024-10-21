#include "volk.h"

// Helper for robustly executing the two-call pattern
template <typename T, typename F, typename... Ts>
auto get_vector(std::vector<T> &out, F &&f, Ts &&...ts) -> VkResult {
	uint32_t count = 0;
	VkResult err;
	do {
		err = f(ts..., &count, nullptr);
		if (err != VK_SUCCESS) {
			return err;
		};
		out.resize(count);
		err = f(ts..., &count, out.data());
		out.resize(count);
	} while (err == VK_INCOMPLETE);
	return err;
}

template <typename T, typename F, typename... Ts>
auto get_vector_noerror(F &&f, Ts &&...ts) -> std::vector<T> {
	uint32_t count = 0;
	std::vector<T> results;
	f(ts..., &count, nullptr);
	results.resize(count);
	f(ts..., &count, results.data());
	results.resize(count);
	return results;
}