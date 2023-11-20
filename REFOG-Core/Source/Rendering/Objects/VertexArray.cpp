#include "REFOG/Rendering/Objects/VertexArray.h"
#include "REFOG/Logger.h"
#include <stdint.h>

namespace REFOG {
	GLsizei VertexArray::GetVertexElementsSize(std::vector<VertexElement> elements) {
		GLsizei size = 0;

		for (int i = 0; i < elements.size(); i++) {
			switch (elements[i])
			{
			case VertexElement::Float:
			case VertexElement::Int:
				size += sizeof(float);
				break;
			case VertexElement::Vec2:
				size += sizeof(float) * 2;
				break;
			case VertexElement::Vec3:
				size += sizeof(float) * 3;
				break;
			case VertexElement::Vec4:
				size += sizeof(float) * 4;
				break;
			default:
				break;
			}
		}

		return size;
	}


	VertexArray::VertexArray() {
		glCreateVertexArrays(1, &m_VertexArray);
	}

	VertexArray::~VertexArray() {
		glDeleteVertexArrays(1, &m_VertexArray);
	}

	void VertexArray::Bind() {
		glBindVertexArray(m_VertexArray);
	}

	void VertexArray::Unbind() {
		glBindVertexArray(0);
	}

	void VertexArray::SetData(std::vector<VertexElement> elements) {
		m_Stride = GetVertexElementsSize(elements);
		uint32_t offset = 0;
		for (GLuint i = 0; i < elements.size(); i++) {
			switch (elements[i])
			{
			case VertexElement::Int:
				glEnableVertexArrayAttrib(m_VertexArray, i);
				glVertexArrayAttribBinding(m_VertexArray, i, 0);
				glVertexArrayAttribFormat(m_VertexArray, i, 1, GL_INT, GL_FALSE, offset);
				offset += sizeof(int);
#ifdef VERTEX_ARRAY_DEBUG
				REFOG_INFO("Added Int Attrib");
#endif
				break;
			case VertexElement::Float:
				glEnableVertexArrayAttrib(m_VertexArray, i);
				glVertexArrayAttribBinding(m_VertexArray, i, 0);
				glVertexArrayAttribFormat(m_VertexArray, i, 1, GL_FLOAT, GL_FALSE, offset);
				offset += sizeof(float);
#ifdef VERTEX_ARRAY_DEBUG
				REFOG_INFO("Added Float Attrib");
#endif
				break;
			case VertexElement::Vec2:
				glEnableVertexArrayAttrib(m_VertexArray, i);
				glVertexArrayAttribBinding(m_VertexArray, i, 0);
				glVertexArrayAttribFormat(m_VertexArray, i, 2, GL_FLOAT, GL_FALSE, offset);
				offset += sizeof(float) * 2;
#ifdef VERTEX_ARRAY_DEBUG
				REFOG_INFO("Added Vec2 Attrib");
#endif
				break;
			case VertexElement::Vec3:
				glEnableVertexArrayAttrib(m_VertexArray, i);
				glVertexArrayAttribBinding(m_VertexArray, i, 0);
				glVertexArrayAttribFormat(m_VertexArray, i, 3, GL_FLOAT, GL_FALSE, offset);
				offset += sizeof(float) * 3;
#ifdef VERTEX_ARRAY_DEBUG
				REFOG_INFO("Added Vec3 Attrib");
#endif
				break;
			case VertexElement::Vec4:
				glEnableVertexArrayAttrib(m_VertexArray, i);
				glVertexArrayAttribBinding(m_VertexArray, i, 0);
				glVertexArrayAttribFormat(m_VertexArray, i, 4, GL_FLOAT, GL_FALSE, offset);
				offset += sizeof(float) * 4;
#ifdef VERTEX_ARRAY_DEBUG
				REFOG_INFO("Added Vec4 Attrib");
#endif
				break;
			default:
				break;
			}
		}
	}

	void VertexArray::AddVertexBuffer(Buffer& buffer) {
		glVertexArrayVertexBuffer(m_VertexArray, 0, buffer, 0, m_Stride);
	}

	void VertexArray::AddElementBuffer(Buffer& buffer) {
		glVertexArrayElementBuffer(m_VertexArray, buffer);
	}
}