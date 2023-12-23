#pragma once
#include "REFOG/Vulkan/Objects/Priority.h"
#include "REFOG/Vulkan/Utils.h"

namespace REFOG {
	extern class Window;

	namespace Vulkan {
		extern class Surface;
		extern class Device;
		extern struct QueueFamilyIndices;

		struct SwapchainDetails {
			VkSurfaceCapabilitiesKHR Capabilities;
			std::vector<VkSurfaceFormatKHR> Formats;
			std::vector<VkPresentModeKHR> PresentModes;
			Surface* Surface;
		};

		struct SwapchainCreateInfo {
			Window& Window;
			const Device& Device;
		};
		
		class Swapchain : public Priority {
		private:
			VkSwapchainKHR m_Swapchain = VK_NULL_HANDLE;
			std::vector<VkImage> m_SwapchainImages;
			VkFormat m_SwapchainFormat;
			VkDevice m_Device;
		public:
			Swapchain(Window&, const Device&);
			virtual ~Swapchain();

			void Destroy() override;

			static SwapchainDetails QueryDetails(VkPhysicalDevice, Surface&);
			static VkSurfaceFormatKHR ChooseFormat(std::vector<VkSurfaceFormatKHR>);
			static VkPresentModeKHR ChoosePresentMode(std::vector<VkPresentModeKHR>);
			static VkExtent2D ChooseExtent(const VkSurfaceCapabilitiesKHR&, Window);
		};
	}
}