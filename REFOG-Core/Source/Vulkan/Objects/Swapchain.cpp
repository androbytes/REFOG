#include "REFOG/Vulkan/Objects/Swapchain.h"
#include "REFOG/Vulkan/Objects/Surface.h"
#include "REFOG/Vulkan/Objects/Device.h"
#include "REFOG/Window.h"
#include <vulkan/vk_enum_string_helper.h>

namespace REFOG {
	namespace Vulkan {
		Swapchain::Swapchain(Window& Window, const Device& Device) 
			: Priority(3) {
			const SwapchainDetails Details = Device.GetSwapchainDetails();

			VkSurfaceFormatKHR SurfaceFormat = ChooseFormat(Details.Formats);
			VkPresentModeKHR PresentMode = ChoosePresentMode(Details.PresentModes);
			VkExtent2D Extent = ChooseExtent(Details.Capabilities, Window);
			QueueFamilyIndices Indices = Device.m_FamilyIndices;

			uint32_t ImageCount = Details.Capabilities.minImageCount + 1;
			if (Details.Capabilities.maxImageCount > 0 && ImageCount > Details.Capabilities.maxImageCount) {
				ImageCount = Details.Capabilities.maxImageCount;
			}

			VkSwapchainCreateInfoKHR SwapchainCI{};
			SwapchainCI.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			SwapchainCI.surface          = Details.Surface->m_Surface;
			SwapchainCI.minImageCount    = ImageCount;
			SwapchainCI.imageFormat      = SurfaceFormat.format;
			SwapchainCI.imageColorSpace  = SurfaceFormat.colorSpace;
			SwapchainCI.imageExtent      = Extent;
			SwapchainCI.imageArrayLayers = 1;
			SwapchainCI.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

			if (Indices.GraphicsFamily.value() != Indices.PresentFamily.value()) {
				uint32_t FamilyIndices[] = { Indices.GraphicsFamily.value(), Indices.PresentFamily.value() };
				SwapchainCI.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
				SwapchainCI.queueFamilyIndexCount = 2;
				SwapchainCI.pQueueFamilyIndices   = FamilyIndices;
			}
			else {
				SwapchainCI.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
				SwapchainCI.queueFamilyIndexCount = 0;
				SwapchainCI.pQueueFamilyIndices   = nullptr;
			}

			SwapchainCI.preTransform   = Details.Capabilities.currentTransform;
			SwapchainCI.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			SwapchainCI.presentMode    = PresentMode;
			SwapchainCI.clipped        = VK_TRUE;
			SwapchainCI.oldSwapchain   = VK_NULL_HANDLE;

			m_SwapchainFormat = SurfaceFormat.format;
			m_Device = Device.m_LogicalDevice;

			VK_CHECK(vkCreateSwapchainKHR(Device.m_LogicalDevice, &SwapchainCI, nullptr, &m_Swapchain));
		}

		Swapchain::~Swapchain() { }

		void Swapchain::Destroy() {
			if (m_Swapchain == VK_NULL_HANDLE) return;
			vkDestroySwapchainKHR(m_Device, m_Swapchain, nullptr);
		}

		SwapchainDetails Swapchain::QueryDetails(VkPhysicalDevice PhysicalDevice, Surface& Surface) {
			SwapchainDetails Details{};

			VkSurfaceCapabilitiesKHR Capabilities{};
			VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(PhysicalDevice, Surface.m_Surface, &Capabilities));
			Details.Capabilities = Capabilities;
			
			uint32_t FormatCount;
			vkGetPhysicalDeviceSurfaceFormatsKHR(PhysicalDevice, Surface.m_Surface, &FormatCount, nullptr);
			Details.Formats.resize(FormatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(PhysicalDevice, Surface.m_Surface, &FormatCount, Details.Formats.data());
			if (FormatCount == 0) {
				REFOG_CRITICAL("Format Count is 0!");
				return {};
			}

			uint32_t PresentModeCount;
			vkGetPhysicalDeviceSurfacePresentModesKHR(PhysicalDevice, Surface.m_Surface, &PresentModeCount, nullptr);
			Details.PresentModes.resize(PresentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(PhysicalDevice, Surface.m_Surface, &PresentModeCount, Details.PresentModes.data());
			if (PresentModeCount == 0) {
				REFOG_CRITICAL("Present Mode Count is 0!");
				return {};
			}

			Details.Surface = &Surface;

			return Details;
		}

		VkSurfaceFormatKHR Swapchain::ChooseFormat(std::vector<VkSurfaceFormatKHR> SurfaceFormats) {
			for (VkSurfaceFormatKHR SurfaceFormat : SurfaceFormats) {
				if (SurfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB && SurfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
					return SurfaceFormat;
				}
			}

			REFOG_WARN("Could not find wanted format & color space");
			return SurfaceFormats[0];
		}

		VkPresentModeKHR Swapchain::ChoosePresentMode(std::vector<VkPresentModeKHR> PresentModes) {
			for (VkPresentModeKHR PresentMode : PresentModes) {
				if (PresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
					return PresentMode;
				}
			}

			REFOG_WARN("Could not find wanted present mode");
			return VK_PRESENT_MODE_FIFO_KHR;
		}

		VkExtent2D Swapchain::ChooseExtent(const VkSurfaceCapabilitiesKHR& Capabilities, Window Window) {
			if (Capabilities.currentExtent.width != std::numeric_limits<uint32_t>().max()) {
				return Capabilities.currentExtent;
			}
			
			glm::vec2 WindowSize = Window.GetWindowSize();
			VkExtent2D Extent{
				static_cast<uint32_t>(WindowSize.x),
				static_cast<uint32_t>(WindowSize.y)
			};

			Extent.width = std::clamp(Extent.width, Capabilities.minImageExtent.width, Capabilities.maxImageExtent.width);
			Extent.height = std::clamp(Extent.height, Capabilities.minImageExtent.height, Capabilities.maxImageExtent.height);

			return Extent;
		}
	}
}