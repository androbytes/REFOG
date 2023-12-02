#include "REFOG/Vulkan/Objects/Validation.h"
#include "REFOG/Vulkan/Objects/Instance.h"
#include "REFOG/Vulkan/Utils.h"
#include "REFOG/Logger.h"

namespace REFOG {
	namespace Vulkan {
		VKAPI_ATTR VkBool32 VKAPI_CALL Validation::DebugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData) {

			if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
				REFOG_WARN("Validation layer: {}", pCallbackData->pMessage);
			} else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
				REFOG_ERROR("Validation layer: {}", pCallbackData->pMessage);
			} else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT){
				REFOG_INFO("Validation layer: {}", pCallbackData->pMessage);
			}

			return VK_FALSE;
		}

		void Validation::CreateDebugger(VkInstance instance) {
			m_Instance = instance;
			//vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance, "vkCreateDebugUtilsMessengerEXT");
			//vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance, "vkDestroyDebugUtilsMessengerEXT");
			
			VK_CHECK_FN(vkCreateDebugUtilsMessengerEXT);
			VK_CHECK_FN(vkDestroyDebugUtilsMessengerEXT);

			m_ExtensionProperties = std::vector<VkExtensionProperties>();
			m_LayerProperties = std::vector<VkLayerProperties>();

			VK_CHECK(vkCreateDebugUtilsMessengerEXT(m_Instance, &m_DebugMessengerCI, nullptr, &m_DebugMessenger));
		}

		Validation::Validation()
			: Priority(2) {
			m_LayerProperties = std::vector<VkLayerProperties>();
			m_Instance = nullptr;

			m_DebugMessengerCI = {};
			m_DebugMessengerCI.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			m_DebugMessengerCI.pfnUserCallback = DebugCallback;
			m_DebugMessengerCI.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			m_DebugMessengerCI.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
				| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		}

		Validation::~Validation() { }

		void Validation::Destroy() {
			vkDestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
		}

		bool Validation::CheckExtension(const char* extName) {
			if (m_ExtensionProperties.empty()) {
				uint32_t count = 0;
				VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr));
				m_ExtensionProperties.resize(count);
				VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &count, m_ExtensionProperties.data()));
			}

			for (VkExtensionProperties extension : m_ExtensionProperties) {
				if (strcmp(extension.extensionName, extName)) {
					return true;
				}
			}

			return false;
		}

		bool Validation::CheckLayer(const char* layerName) {
			if (m_LayerProperties.empty()) {
				uint32_t layerCount;
				vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
				m_LayerProperties.resize(layerCount);

				vkEnumerateInstanceLayerProperties(&layerCount, m_LayerProperties.data());
			}

			for (const VkLayerProperties& layerProperty : m_LayerProperties) {
				if (strcmp(layerName, layerProperty.layerName)) {
					return true;
				}
			}

			return false;
		}
	}
}