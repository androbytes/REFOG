#include "REFOG/Rendering/Objects/Buffer.h"

namespace REFOG {
	Buffer::Buffer() {
		glCreateBuffers(1, &m_Buffer);
	}

	Buffer::~Buffer() {
		glDeleteBuffers(1, &m_Buffer);
	}

	void Buffer::SetData(void* data, GLsizei dataSize) {
		glNamedBufferData(m_Buffer, dataSize, data, GL_DYNAMIC_DRAW);
		m_DataSize = dataSize;
	}

	void Buffer::SetSubData(void* data) {
		glNamedBufferSubData(m_Buffer, 0, m_DataSize, data);
	}

	GLsizei Buffer::GetDataSize() {
		return m_DataSize;
	}

	void* Buffer::GetBufferData() {
		void* data = new void*;
		glGetNamedBufferSubData(m_Buffer, 0, GetDataSize(), data);
		return data;
	}
}