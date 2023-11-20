#pragma once
#include <stdint.h>

namespace REFOG {
	extern class SpriteBatch;

	class SpriteBatchObject {
	private:
		uint64_t m_ObjectIndex;
		SpriteBatch& m_SpriteBatch;
	public:
		SpriteBatchObject(SpriteBatch& spriteBatch, uint64_t index)
			: m_SpriteBatch(spriteBatch), m_ObjectIndex(index) {}
		virtual ~SpriteBatchObject();
	};

	typedef SpriteBatchObject BatchObject;
}