#include "REFOG/Vulkan/Objects/Priority.h"

namespace REFOG {
	namespace Vulkan {
		std::priority_queue<Priority*, std::vector<Priority*>, Priority> Priority::s_Queue = {};
	}

	void Vulkan::Cleanup() {
		while (!Priority::s_Queue.empty()) {
			Priority::s_Queue.top()->Destroy();
			Priority::s_Queue.pop();
		}
	}
}