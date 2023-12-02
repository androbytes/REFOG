#include "REFOG/Rendering/TextRenderer.h"
#include "REFOG/Rendering/Objects/Texture.h"
#include "REFOG/Rendering/Camera.h"
#include <filesystem>

namespace REFOG {
	TextRenderer::TextRenderer(Font& font) {
		m_Font = font;

		m_VertexBuffer.SetData(NULL, sizeof(float) * 6 * 4); // 6 vertices, 4 different data per vertex
		
		m_VertexArray.SetData({ Vec4 });
		m_VertexArray.AddVertexBuffer(m_VertexBuffer);

		m_Shader.LoadShader("\\Shaders\\TextShader.vertex.glsl", Vertex);
		m_Shader.LoadShader("\\Shaders\\TextShader.fragment.glsl", Fragment);
		m_Shader.LinkProgram();
	}

	void TextRenderer::Render(TextInfo info) {
		m_Shader.SetVec3("TextColor", info.Color);
		m_Shader.Use();
		glActiveTexture(GL_TEXTURE0);
		m_VertexArray.Bind();

		for (std::string::const_iterator c = info.Text.begin(); c != info.Text.end(); c++) {
			Character character = m_Font.GetChar(*c);
			
			float xPos = info.Position.x + character.Bearing.x * info.Scale;
			float yPos = info.Position.y - (character.Size.y - character.Bearing.y) * info.Scale;

			float width = character.Size.x * info.Scale;
			float height = character.Size.y * info.Scale;

			float vertices[6][4] = {
				{ xPos,         yPos + height, 0.0f, 0.0f },
				{ xPos,         yPos,          0.0f, 1.0f },
				{ xPos + width, yPos,          1.0f, 1.0f },

				{ xPos,         yPos + height, 0.0f, 0.0f },
				{ xPos + width, yPos,          1.0f, 1.0f },
				{ xPos + width, yPos + height, 1.0f, 0.0f }
			};

			character.Texture->SetUnit(0);
			m_VertexBuffer.SetSubData(vertices);

			glDrawArrays(GL_TRIANGLES, 0, 6);

			info.Position.x += (character.Advance >> 6) * info.Scale;
		}

		m_VertexArray.Unbind();
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void TextRenderer::UpdateShader() {
		m_Shader.SetMat4("Projection", REFOG::GlobalCamera->GetProjection());
	}
}