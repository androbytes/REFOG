#pragma once
#include "Priority.h"
#include "Validation.h"

#define REFOG_VERSION VK_MAKE_API_VERSION(1, 0, 0, 0)

namespace REFOG {
	namespace Vulkan {
		struct InstanceProps {
			uint32_t AppVersion    = VK_MAKE_API_VERSION(1, 0, 0, 0); // Use VK_MAKE_API_VERSION
			const char* AppName    = "REFOG App";
			Validation* Validation = nullptr;
		};

		class Instance : public Priority {
		private:
			friend class Device;
			friend class Surface;
			VkInstance m_Instance;

			std::vector<const char*> FindExtensions(Validation*);
		public:
			Instance(InstanceProps);
			virtual ~Instance();

			void Destroy() override;
		};
	}
}