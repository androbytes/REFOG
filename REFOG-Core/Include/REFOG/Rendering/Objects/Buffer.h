#pragma once
#include "glad/glad.h"

namespace REFOG {
	class Buffer {
	private:
		GLuint m_Buffer;
		GLsizei m_DataSize;
	public:
		Buffer();
		virtual ~Buffer();

		void SetData(void*, GLsizei);
		void SetSubData(void*);
		GLsizei GetDataSize(); // array size
		void* GetBufferData();

		operator GLuint() { return m_Buffer; }
	};
}
