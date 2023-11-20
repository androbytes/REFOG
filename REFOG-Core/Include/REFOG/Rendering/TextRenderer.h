#pragma once
#include "Font.h"
#include "Objects/Objects.h"

namespace REFOG {
	struct TextInfo {
		std::string Text;
		glm::vec2 Position;
		float Scale;
		glm::vec3 Color;
	};

	class TextRenderer {
	private:
		VertexArray m_VertexArray;
		Buffer m_VertexBuffer;
		Shader m_Shader;
		Font m_Font;
	public:
		TextRenderer(Font&);
		
		void Render(TextInfo);
		void UpdateShader(); // Updates perspective uniform in text shader
	};
}