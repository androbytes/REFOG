#include "REFOG/Rendering/Batching/Batching.h"

namespace REFOG {
	SpriteBatchObject::~SpriteBatchObject() {
		m_SpriteBatch.RemoveObject(m_ObjectIndex);
	}
}