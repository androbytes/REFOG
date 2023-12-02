#pragma once
#include <queue>

namespace REFOG {
	namespace Vulkan {
		class Priority {
		public:
			static std::priority_queue < Priority*, std::vector<Priority*>, Priority> s_Queue;
			int PriorityIdx;

			Priority(int idx)
				: PriorityIdx(idx) {
				s_Queue.push(this);
			}
			Priority()
				: PriorityIdx(0) { }
			virtual ~Priority() = default;

			virtual void Destroy() {}

			inline bool operator>(const Priority& other) const  {
				return PriorityIdx > other.PriorityIdx;
			}
			inline bool operator<(const Priority& other) const  {
				return PriorityIdx < other.PriorityIdx;
			}
			inline bool operator ()(const Priority* lhs, const Priority* rhs) const
			{
				return *lhs < *rhs;
			}
		};

		void Cleanup();
	}
}