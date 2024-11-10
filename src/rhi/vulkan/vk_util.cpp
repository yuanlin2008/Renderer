#include "vk_util.h"

#include <map>

#define VK_RESULT(R) { R, #R }
static std::map<VkResult, std::string> result_map = {
	VK_RESULT(VK_SUCCESS),
	VK_RESULT(VK_NOT_READY),
	VK_RESULT(VK_TIMEOUT),
	VK_RESULT(VK_EVENT_SET),
	VK_RESULT(VK_EVENT_RESET),
	VK_RESULT(VK_INCOMPLETE),
	VK_RESULT(VK_ERROR_OUT_OF_HOST_MEMORY),
	VK_RESULT(VK_ERROR_OUT_OF_DEVICE_MEMORY),
	VK_RESULT(VK_ERROR_INITIALIZATION_FAILED),
	VK_RESULT(VK_ERROR_DEVICE_LOST),
	VK_RESULT(VK_ERROR_MEMORY_MAP_FAILED),
	VK_RESULT(VK_ERROR_LAYER_NOT_PRESENT),
	VK_RESULT(VK_ERROR_EXTENSION_NOT_PRESENT),
	VK_RESULT(VK_ERROR_FEATURE_NOT_PRESENT),
	VK_RESULT(VK_ERROR_INCOMPATIBLE_DRIVER),
	VK_RESULT(VK_ERROR_TOO_MANY_OBJECTS),
	VK_RESULT(VK_ERROR_FORMAT_NOT_SUPPORTED),
	VK_RESULT(VK_ERROR_FRAGMENTED_POOL),
	VK_RESULT(VK_ERROR_UNKNOWN),
	VK_RESULT(VK_ERROR_OUT_OF_POOL_MEMORY),
	VK_RESULT(VK_ERROR_INVALID_EXTERNAL_HANDLE),
	VK_RESULT(VK_ERROR_FRAGMENTATION),
	VK_RESULT(VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS),
	VK_RESULT(VK_PIPELINE_COMPILE_REQUIRED),
	VK_RESULT(VK_ERROR_SURFACE_LOST_KHR),
	VK_RESULT(VK_ERROR_NATIVE_WINDOW_IN_USE_KHR),
	VK_RESULT(VK_SUBOPTIMAL_KHR),
	VK_RESULT(VK_ERROR_OUT_OF_DATE_KHR),
	VK_RESULT(VK_ERROR_INCOMPATIBLE_DISPLAY_KHR),
	VK_RESULT(VK_ERROR_VALIDATION_FAILED_EXT),
	VK_RESULT(VK_ERROR_INVALID_SHADER_NV),
	VK_RESULT(VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR),
	VK_RESULT(VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR),
	VK_RESULT(VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR),
	VK_RESULT(VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR),
	VK_RESULT(VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR),
	VK_RESULT(VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR),
	VK_RESULT(VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT),
	VK_RESULT(VK_ERROR_NOT_PERMITTED_KHR),
	VK_RESULT(VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT),
	VK_RESULT(VK_THREAD_IDLE_KHR),
	VK_RESULT(VK_THREAD_DONE_KHR),
	VK_RESULT(VK_OPERATION_DEFERRED_KHR),
	VK_RESULT(VK_OPERATION_NOT_DEFERRED_KHR),
	VK_RESULT(VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR),
	VK_RESULT(VK_ERROR_COMPRESSION_EXHAUSTED_EXT),
	VK_RESULT(VK_ERROR_INCOMPATIBLE_SHADER_BINARY_EXT),
};

std::string get_result_str(VkResult r) {
	if (auto it = result_map.find(r); it != result_map.end()) {
		return it->second;
	}
	return "UNKNOWN_ERROR";
}

VkFormat get_format(RHIFormat format) {
	switch (format) {
		case RHIFormat::R32G32B32A32_TYPELESS:
			return VK_FORMAT_R32G32B32A32_SFLOAT;
		case RHIFormat::R32G32B32_TYPELESS:
			return VK_FORMAT_R32G32B32_SFLOAT;
		case RHIFormat::R32G32_TYPELESS:
			return VK_FORMAT_R32G32_SFLOAT;
		case RHIFormat::R32_TYPELESS:
			return VK_FORMAT_R32_SFLOAT;

		case RHIFormat::R16G16B16A16_TYPELESS:
			return VK_FORMAT_R16G16B16A16_SFLOAT;
		case RHIFormat::R16G16_TYPELESS:
			return VK_FORMAT_R16G16_SFLOAT;
		case RHIFormat::R16_TYPELESS:
			return VK_FORMAT_R16_SFLOAT;

		case RHIFormat::R8G8B8A8_TYPELESS:
			return VK_FORMAT_R8G8B8A8_UNORM;
		case RHIFormat::R8G8_TYPELESS:
			return VK_FORMAT_R8G8_UNORM;
		case RHIFormat::R8_TYPELESS:
			return VK_FORMAT_R8_UNORM;
		case RHIFormat::B8G8R8A8_TYPELESS:
			return VK_FORMAT_B8G8R8A8_UNORM;

		case RHIFormat::R64_UINT:
			return VK_FORMAT_R64_UINT;

		case RHIFormat::R32G32B32A32_FLOAT:
			return VK_FORMAT_R32G32B32A32_SFLOAT;
		case RHIFormat::R32G32B32_FLOAT:
			return VK_FORMAT_R32G32B32_SFLOAT;
		case RHIFormat::R32G32_FLOAT:
			return VK_FORMAT_R32G32_SFLOAT;
		case RHIFormat::R32_FLOAT:
			return VK_FORMAT_R32_SFLOAT;

		case RHIFormat::R16G16B16A16_FLOAT:
			return VK_FORMAT_R16G16B16A16_SFLOAT;
		case RHIFormat::R16G16_FLOAT:
			return VK_FORMAT_R16G16_SFLOAT;
		case RHIFormat::R16_FLOAT:
			return VK_FORMAT_R16_SFLOAT;

		case RHIFormat::R32G32B32A32_UINT:
			return VK_FORMAT_R32G32B32A32_UINT;
		case RHIFormat::R32G32B32_UINT:
			return VK_FORMAT_R32G32B32_UINT;
		case RHIFormat::R32G32_UINT:
			return VK_FORMAT_R32G32_UINT;
		case RHIFormat::R32_UINT:
			return VK_FORMAT_R32_UINT;

		case RHIFormat::R16G16B16A16_UINT:
			return VK_FORMAT_R16G16B16A16_UINT;
		case RHIFormat::R16G16_UINT:
			return VK_FORMAT_R16G16_UINT;
		case RHIFormat::R16_UINT:
			return VK_FORMAT_R16_UINT;

		case RHIFormat::R8G8B8A8_UINT:
			return VK_FORMAT_R8G8B8A8_UINT;
		case RHIFormat::R8G8_UINT:
			return VK_FORMAT_R8G8_UINT;
		case RHIFormat::R8_UINT:
			return VK_FORMAT_R8_UINT;

		case RHIFormat::R64_SINT:
			return VK_FORMAT_R64_SINT;

		case RHIFormat::R32G32B32A32_SINT:
			return VK_FORMAT_R32G32B32A32_SINT;
		case RHIFormat::R32G32B32_SINT:
			return VK_FORMAT_R32G32B32_SINT;
		case RHIFormat::R32G32_SINT:
			return VK_FORMAT_R32G32_SINT;
		case RHIFormat::R32_SINT:
			return VK_FORMAT_R32_SINT;

		case RHIFormat::R16G16B16A16_SINT:
			return VK_FORMAT_R16G16B16A16_SINT;
		case RHIFormat::R16G16_SINT:
			return VK_FORMAT_R16G16_SINT;
		case RHIFormat::R16_SINT:
			return VK_FORMAT_R16_SINT;

		case RHIFormat::R8G8B8A8_SINT:
			return VK_FORMAT_R8G8B8A8_SINT;
		case RHIFormat::R8G8_SINT:
			return VK_FORMAT_R8G8_SINT;
		case RHIFormat::R8_SINT:
			return VK_FORMAT_R8_SINT;

		case RHIFormat::R16G16B16A16_UNORM:
			return VK_FORMAT_R16G16B16A16_UNORM;
		case RHIFormat::R16G16_UNORM:
			return VK_FORMAT_R16G16_UNORM;
		case RHIFormat::R16_UNORM:
			return VK_FORMAT_R16_UNORM;

		case RHIFormat::R8G8B8A8_UNORM:
			return VK_FORMAT_R8G8B8A8_UNORM;
		case RHIFormat::R8G8B8A8_UNORM_SRGB:
			return VK_FORMAT_R8G8B8A8_SRGB;
		case RHIFormat::R8G8_UNORM:
			return VK_FORMAT_R8G8_UNORM;
		case RHIFormat::R8_UNORM:
			return VK_FORMAT_R8_UNORM;
		case RHIFormat::B8G8R8A8_UNORM:
			return VK_FORMAT_B8G8R8A8_UNORM;
		case RHIFormat::B8G8R8A8_UNORM_SRGB:
			return VK_FORMAT_B8G8R8A8_SRGB;
		case RHIFormat::B8G8R8X8_UNORM:
			return VK_FORMAT_B8G8R8A8_UNORM;
		case RHIFormat::B8G8R8X8_UNORM_SRGB:
			return VK_FORMAT_B8G8R8A8_SRGB;

		case RHIFormat::R16G16B16A16_SNORM:
			return VK_FORMAT_R16G16B16A16_SNORM;
		case RHIFormat::R16G16_SNORM:
			return VK_FORMAT_R16G16_SNORM;
		case RHIFormat::R16_SNORM:
			return VK_FORMAT_R16_SNORM;

		case RHIFormat::R8G8B8A8_SNORM:
			return VK_FORMAT_R8G8B8A8_SNORM;
		case RHIFormat::R8G8_SNORM:
			return VK_FORMAT_R8G8_SNORM;
		case RHIFormat::R8_SNORM:
			return VK_FORMAT_R8_SNORM;

		case RHIFormat::D32_FLOAT:
			return VK_FORMAT_D32_SFLOAT;
		case RHIFormat::D16_UNORM:
			return VK_FORMAT_D16_UNORM;
		case RHIFormat::D32_FLOAT_S8_UINT:
			return VK_FORMAT_D32_SFLOAT_S8_UINT;
		case RHIFormat::R32_FLOAT_X32_TYPELESS:
			return VK_FORMAT_R32_SFLOAT;

		case RHIFormat::B4G4R4A4_UNORM:
			return VK_FORMAT_A4R4G4B4_UNORM_PACK16_EXT;
		case RHIFormat::B5G6R5_UNORM:
			return VK_FORMAT_R5G6B5_UNORM_PACK16;
		case RHIFormat::B5G5R5A1_UNORM:
			return VK_FORMAT_A1R5G5B5_UNORM_PACK16;

		case RHIFormat::R9G9B9E5_SHAREDEXP:
			return VK_FORMAT_E5B9G9R9_UFLOAT_PACK32;
		case RHIFormat::R10G10B10A2_TYPELESS:
			return VK_FORMAT_A2B10G10R10_UINT_PACK32;
		case RHIFormat::R10G10B10A2_UINT:
			return VK_FORMAT_A2B10G10R10_UINT_PACK32;
		case RHIFormat::R10G10B10A2_UNORM:
			return VK_FORMAT_A2B10G10R10_UNORM_PACK32;
		case RHIFormat::R11G11B10_FLOAT:
			return VK_FORMAT_B10G11R11_UFLOAT_PACK32;

		case RHIFormat::BC1_UNORM:
			return VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
		case RHIFormat::BC1_UNORM_SRGB:
			return VK_FORMAT_BC1_RGBA_SRGB_BLOCK;
		case RHIFormat::BC2_UNORM:
			return VK_FORMAT_BC2_UNORM_BLOCK;
		case RHIFormat::BC2_UNORM_SRGB:
			return VK_FORMAT_BC2_SRGB_BLOCK;
		case RHIFormat::BC3_UNORM:
			return VK_FORMAT_BC3_UNORM_BLOCK;
		case RHIFormat::BC3_UNORM_SRGB:
			return VK_FORMAT_BC3_SRGB_BLOCK;
		case RHIFormat::BC4_UNORM:
			return VK_FORMAT_BC4_UNORM_BLOCK;
		case RHIFormat::BC4_SNORM:
			return VK_FORMAT_BC4_SNORM_BLOCK;
		case RHIFormat::BC5_UNORM:
			return VK_FORMAT_BC5_UNORM_BLOCK;
		case RHIFormat::BC5_SNORM:
			return VK_FORMAT_BC5_SNORM_BLOCK;
		case RHIFormat::BC6H_UF16:
			return VK_FORMAT_BC6H_UFLOAT_BLOCK;
		case RHIFormat::BC6H_SF16:
			return VK_FORMAT_BC6H_SFLOAT_BLOCK;
		case RHIFormat::BC7_UNORM:
			return VK_FORMAT_BC7_UNORM_BLOCK;
		case RHIFormat::BC7_UNORM_SRGB:
			return VK_FORMAT_BC7_SRGB_BLOCK;

		default:
			return VK_FORMAT_UNDEFINED;
	}
}