#include "REFOG/Vulkan/Objects/Device.h"
#include "REFOG/Vulkan/Objects/Swapchain.h"
#include "REFOG/Vulkan/Objects/Surface.h"
#include "REFOG/Vulkan/Utils.h"
#include <set>

namespace REFOG {
	namespace Vulkan {
		bool Device::CheckDeviceExtension(VkPhysicalDevice PhysicalDevice, const char* Extension) {
			uint32_t Count;
			VkExtensionProperties Properties[1];
			vkEnumerateDeviceExtensionProperties(PhysicalDevice, Extension, &Count, Properties);
			return Count != 0 || Properties == nullptr;
		}

		VkPhysicalDevice Device::ChoosePhysicalDevice(std::vector<VkPhysicalDevice> PhysicalDevices, std::vector<const char*> DeviceExtensions, Surface Surface) {
			std::vector<uint32_t> DeviceScores(PhysicalDevices.size());
			for (int i = 0; i < PhysicalDevices.size(); i++) {
				for (const char* DeviceExtension : DeviceExtensions) {
					if (!CheckDeviceExtension(PhysicalDevices[i], DeviceExtension)) {
						continue;
					}
				}

				QueueFamilyIndices Indices = FindQueueFamilies(PhysicalDevices[i], Surface);
				if (!Indices.complete()) continue;

				const SwapchainDetails& Details = Swapchain::QueryDetails(PhysicalDevices[i], Surface);
				if (Details.Formats.empty()) continue;
				if (Details.PresentModes.empty()) continue;

				VkPhysicalDeviceProperties DeviceProps;
				vkGetPhysicalDeviceProperties(PhysicalDevices[i], &DeviceProps);
				if (DeviceProps.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
					DeviceScores[i] += 1000;
				}
				DeviceScores[i] += DeviceProps.limits.maxImageDimension2D;
				DeviceScores[i] += DeviceProps.limits.maxFramebufferWidth * DeviceProps.limits.maxFramebufferHeight;
				DeviceScores[i] += DeviceProps.limits.maxImageDimensionCube;
			}

			uint32_t Highest = 0;
			int HighestIdx   = 0;
			for (int i = 0; i < DeviceScores.size(); i++) {
				if (DeviceScores[i] == 0) continue;
				if (DeviceScores[i] > Highest) {
					Highest = DeviceScores[i];
					HighestIdx = i;
				}
			}

			if (Highest == 0) {
				REFOG_CRITICAL("No suitable Vulkan device found!");
				throw std::runtime_error("No suitable Vulkan device found!");
			}

			return PhysicalDevices[HighestIdx];
		}

		QueueFamilyIndices Device::FindQueueFamilies(VkPhysicalDevice PhysicalDevice, Surface Surface) {
			QueueFamilyIndices Indices{};

			uint32_t FamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &FamilyCount, nullptr);
			std::vector<VkQueueFamilyProperties> QueueFamilies(FamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &FamilyCount, QueueFamilies.data());

			int Index = 0;
			for (const VkQueueFamilyProperties& QueueFamily : QueueFamilies) {
				if (QueueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
					Indices.GraphicsFamily = Index;
				}

				if (Surface.GetPresentSupport(PhysicalDevice, Index)) {
					Indices.PresentFamily = Index;
				}

				if (Indices.complete()) {
					break;
				}

				Index++;
			}

			return Indices;
		}

		Device::Device(Instance Instance, Surface Surface)
			: Priority(2), m_Surface(Surface) {
			std::vector<const char*> DeviceExtensions = {
				VK_KHR_SWAPCHAIN_EXTENSION_NAME
			};

			m_Instance = Instance.m_Instance;

			std::vector<VkPhysicalDevice> PhysicalDevices;
			uint32_t DeviceCount;
			VK_CHECK(vkEnumeratePhysicalDevices(m_Instance, &DeviceCount, nullptr));
			PhysicalDevices.resize(DeviceCount);
			VK_CHECK(vkEnumeratePhysicalDevices(m_Instance, &DeviceCount, PhysicalDevices.data()));

			m_PhysicalDevice = ChoosePhysicalDevice(PhysicalDevices, DeviceExtensions, Surface);
			m_FamilyIndices = FindQueueFamilies(m_PhysicalDevice, Surface);

			VkPhysicalDeviceFeatures DeviceFeatures{};

			VkDeviceCreateInfo DeviceCI{};
			DeviceCI.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			DeviceCI.pEnabledFeatures = &DeviceFeatures;
			DeviceCI.enabledExtensionCount = DeviceExtensions.size();
			DeviceCI.ppEnabledExtensionNames = DeviceExtensions.data();

			std::vector<VkDeviceQueueCreateInfo> DeviceQueueCIs{};
			std::set<uint32_t> UniqueQueueFamilies = { m_FamilyIndices.GraphicsFamily.value(), m_FamilyIndices.PresentFamily.value() };

			float QueuePriorities[] = { 1.0f };
			for (uint32_t QueueFamily : UniqueQueueFamilies) {
				VkDeviceQueueCreateInfo DeviceQueueCI{};
				DeviceQueueCI.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				DeviceQueueCI.pQueuePriorities = QueuePriorities;
				DeviceQueueCI.queueCount = 1;
				DeviceQueueCI.queueFamilyIndex = QueueFamily;
				DeviceQueueCIs.push_back(DeviceQueueCI);
			}

			DeviceCI.queueCreateInfoCount = DeviceQueueCIs.size();
			DeviceCI.pQueueCreateInfos = DeviceQueueCIs.data();

			// Older implementations required you to set device extensions to instance
			VK_CHECK(vkCreateDevice(m_PhysicalDevice, &DeviceCI, nullptr, &m_LogicalDevice));
			vkGetDeviceQueue(m_LogicalDevice, m_FamilyIndices.GraphicsFamily.value(), 0, &m_GraphicsQueue);
			vkGetDeviceQueue(m_LogicalDevice, m_FamilyIndices.PresentFamily.value(), 0, &m_PresentQueue);
		}

		Device::~Device() { }

		void Device::Destroy() {
			if (m_LogicalDevice == VK_NULL_HANDLE) return;
			vkDestroyDevice(m_LogicalDevice, nullptr);
		}

		SwapchainDetails Device::GetSwapchainDetails() const {
			return Swapchain::QueryDetails(m_PhysicalDevice, m_Surface);
		}
	}
}