#pragma once
#include "Instance.h"
#include <optional>

namespace REFOG {
	namespace Vulkan {
		extern class Surface;
		extern struct SwapchainDetails;

		struct QueueFamilyIndices {
		public:
			std::optional<uint32_t> GraphicsFamily;
			std::optional<uint32_t> PresentFamily;

			bool complete() {
				return GraphicsFamily.has_value() && PresentFamily.has_value();
			}
		};

		class Device : public Priority {
		private:
			friend class Swapchain;

			VkPhysicalDevice m_PhysicalDevice;
			VkDevice m_LogicalDevice = VK_NULL_HANDLE;
			VkQueue m_GraphicsQueue;
			VkQueue m_PresentQueue;
			QueueFamilyIndices m_FamilyIndices;

			Surface& m_Surface;

			bool CheckDeviceExtension(VkPhysicalDevice, const char*);
			VkPhysicalDevice ChoosePhysicalDevice(std::vector<VkPhysicalDevice>, std::vector<const char*> DeviceExtensions, Surface Surface);
			QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice, Surface);

			VkInstance m_Instance;
		public:
			Device(Instance, Surface);
			virtual ~Device();

			void Destroy() override;

			SwapchainDetails GetSwapchainDetails() const;
		};
	}
}