#pragma once
#include "REFOG/Vulkan/Utils.h"
#include "Priority.h"

namespace REFOG {
	extern class Window;
	
	namespace Vulkan {
		extern class Instance;

		class Surface : public Priority {
		private:
			friend class Swapchain;

			VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
			VkInstance m_Instance;
		public:
			Surface(Instance, Window&);
			virtual ~Surface();

			void Destroy() override;

			VkBool32 GetPresentSupport(VkPhysicalDevice, int);
		};
	}
}