#include "REFOG/Vulkan/Objects/Instance.h"
#include "REFOG/Vulkan/Utils.h"
#include <GLFW/glfw3.h>
#include <vector>

namespace REFOG {
	namespace Vulkan {
		std::vector<const char*> Instance::FindExtensions(Validation* Validation) {
			std::vector<const char*> Extensions;
			uint32_t ExtensionCount;
			const char** extensions = glfwGetRequiredInstanceExtensions(&ExtensionCount);
			Extensions = std::vector<const char*>(extensions, extensions + ExtensionCount);
			if (Validation) {
				Extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
				REFOG_INFO("Extensions:");
				for (const char* name : Extensions) {
					REFOG_INFO("	{}", name);
					if (!Validation->CheckExtension(name)) {
						REFOG_ERROR("Required extension is not present! Extension: {}", name);
					}
				}
			}
			return Extensions;

			// Extra code needed if using MoltenVK
		}

		Instance::Instance(InstanceProps props)
			: Priority(1) {
			static bool VolkInit = false;
			if (!VolkInit) {
				VK_CHECK(volkInitialize());
				VolkInit = true;

				VK_CHECK_FN(vkCreateInstance);
			}

			VkApplicationInfo AppInfo = {};
			AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			AppInfo.apiVersion = VK_API_VERSION_1_3;
			AppInfo.applicationVersion = props.AppVersion;
			AppInfo.pApplicationName = props.AppName;
			AppInfo.engineVersion = REFOG_VERSION;
			AppInfo.pEngineName = "REFOG";

			VkInstanceCreateInfo InstanceCI = {};
			InstanceCI.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			InstanceCI.pApplicationInfo = &AppInfo;

			std::vector<const char*> Layers;
			std::vector<const char*> Extensions = FindExtensions(props.Validation);
			InstanceCI.enabledExtensionCount = Extensions.size();
			InstanceCI.ppEnabledExtensionNames = Extensions.data();
			if (props.Validation != nullptr) {
				Layers.push_back("VK_LAYER_KHRONOS_validation");
				InstanceCI.pNext = &props.Validation->m_DebugMessengerCI;
				for (const char* Layer : Layers) {
					if (!props.Validation->CheckLayer(Layer)) {
						REFOG_ERROR("Layer is not present! Layer: {}", Layer);
					}
				}
			}
			InstanceCI.enabledLayerCount = Layers.size();
			InstanceCI.ppEnabledLayerNames = Layers.data();

			VK_CHECK(vkCreateInstance(&InstanceCI, nullptr, &m_Instance));

			if (m_Instance) {
				volkLoadInstance(m_Instance);
			}

			props.Validation->CreateDebugger(m_Instance);
		}

		Instance::~Instance() { }

		void Instance::Destroy() {
			if (m_Instance) {
				vkDestroyInstance(m_Instance, nullptr);
			}
		}
	}
}