#pragma once
#include "Priority.h"
#include <volk.h>
#include <vector>

namespace REFOG {
	namespace Vulkan {
		extern class Instance;

		class Validation : public Priority {
		private:
			friend class Instance;

			static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
				VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
				VkDebugUtilsMessageTypeFlagsEXT messageType,
				const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
				void* pUserData);

			std::vector<VkExtensionProperties> m_ExtensionProperties;
			std::vector<VkLayerProperties> m_LayerProperties;

			VkDebugUtilsMessengerCreateInfoEXT m_DebugMessengerCI;
			VkDebugUtilsMessengerEXT m_DebugMessenger;
			VkInstance m_Instance;

			void CreateDebugger(VkInstance);
		public:
			Validation();
			virtual ~Validation();

			void Destroy() override;

			bool CheckExtension(const char*);
			bool CheckLayer(const char*);
		};
	}
}
