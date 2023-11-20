#pragma once
#include "Buffer.h"
#include <vector>

namespace REFOG {
	enum VertexElement {
		Int,
		Float,
		Vec2,
		Vec3,
		Vec4
	};

	class VertexArray {
	private:
		GLuint m_VertexArray;
		GLuint m_Stride;
	public:
		static GLsizei GetVertexElementsSize(std::vector<VertexElement>);

		VertexArray();
		virtual ~VertexArray();

		void Bind();
		void Unbind();

		void SetData(std::vector<VertexElement>);
		void AddVertexBuffer(Buffer&);
		void AddElementBuffer(Buffer&);
	};
}