#pragma once
#include <volk.h>
#include <vulkan/vk_enum_string_helper.h>
#include "REFOG/Logger.h"

#define VK_CHECK(func) {\
VkResult result = func; \
if(result != VK_SUCCESS) { \
	REFOG_CRITICAL("Vulkan: Function failed: {0}, Result: {1}", #func, string_VkResult(result)); \
} \
}

#define VK_CHECK_FN(func) {\
if(!func) { \
	REFOG_CRITICAL("{} is nullptr!", #func); \
	throw std::runtime_error("Function is nullptr!"); \
} \
}