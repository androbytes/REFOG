#include "REFOG/Vulkan/Objects/Surface.h"
#include "REFOG/Vulkan/Objects/Device.h"
#include "REFOG/Window.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace REFOG {
	namespace Vulkan {
		Surface::Surface(Instance instance, Window window)
			: Priority(2) {
			m_Instance = instance.m_Instance;
			window.CreateSurface(m_Instance, &m_Surface);

			/*VkWin32SurfaceCreateInfoKHR SurfaceCI{};
			SurfaceCI.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
			SurfaceCI.hwnd = glfwGetWin32Window(window.m_Window);
			SurfaceCI.hinstance = GetModuleHandle(nullptr);

			VK_CHECK_FN(vkCreateWin32SurfaceKHR);
			VK_CHECK_FN(vkDestroySurfaceKHR);

			VK_CHECK(vkCreateWin32SurfaceKHR(instance.m_Instance, &SurfaceCI, nullptr, &m_Surface));*/
		}

		Surface::~Surface() { }

		void Surface::Destroy() {
			vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
		}

		VkBool32 Surface::GetPresentSupport(VkPhysicalDevice Device, int idx) {
			VkBool32 PresentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(Device, idx, m_Surface, &PresentSupport);
			return PresentSupport;
		}
	}
}