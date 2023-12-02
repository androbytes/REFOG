#define VOLK_IMPLEMENTATION
#include "REFOG/Vulkan/Vulkan.h"

using namespace REFOG;

class VulkanTest {
public:
	void Run() {
		Window window("My Vulkan App!", { 1280, 720 }, false);
		Vulkan::Validation Validation{};
		Vulkan::Instance Instance{ Vulkan::InstanceProps { .AppName = "My Vulkan App!", .Validation = &Validation } };
		Vulkan::Surface Surface{ Instance, window };
		Vulkan::Device Device{ Instance, Surface };
		Vulkan::Swapchain Swapchain{ window,  Device };

		Vulkan::Cleanup();
	}
};