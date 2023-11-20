#include "REFOG/Rendering/Mesh.h"

namespace REFOG {
	namespace VertexShaderElements {
		std::vector<VertexElement> DefaultShader = { VertexElement::Vec3, VertexElement::Vec3 };
		std::vector<VertexElement> SpriteShader = { VertexElement::Vec3, VertexElement::Vec3, VertexElement::Vec2 };
	}

	Mesh::Mesh() {
		m_VertexArray = new VertexArray();
		m_VertexBuffer = new Buffer();
		m_IndexBuffer = new Buffer();
		m_Shader = new Shader();
	}

	Mesh::~Mesh() {
		delete m_VertexArray;
		delete m_VertexBuffer;
		delete m_IndexBuffer;
		delete m_Shader;
	}

	void Mesh::SetupShader(ShaderData data) {
		switch(data.File){
		case Default:
			m_Shader->LoadShader("\\Shaders\\DefaultShader.vertex.glsl", Vertex);
			m_Shader->LoadShader("\\Shaders\\DefaultShader.fragment.glsl", Fragment);
			break;
		case Sprite:
			m_Shader->LoadShader("\\Shaders\\SpriteShader.vertex.glsl", Vertex);
			m_Shader->LoadShader("\\Shaders\\SpriteShader.fragment.glsl", Fragment);
			break;
		case Custom:
			for (int i = 0; i < sizeof(data.ShaderFilePaths); i++) {
				if (data.ShaderFilePaths.data() != NULL)
					m_Shader->LoadShader(data.ShaderFilePaths[i], (ShaderType)i);
			}
			break;
		}

		m_Shader->LinkProgram();
	}

	void Mesh::SetupVertexBuffer(void* data, GLsizei dataSize) {
		m_VertexBuffer->SetData(data, dataSize);
	}

	void Mesh::SetupIndexBuffer(void* data, GLsizei dataSize) {
		m_IndexBuffer->SetData(data, dataSize);
	}

	void Mesh::SetupVertexArray(std::vector<VertexElement> elements) {
		m_VertexArray->SetData(elements);
		m_VertexArray->AddVertexBuffer(*m_VertexBuffer);
		m_VertexArray->AddElementBuffer(*m_IndexBuffer);
	}

	void Mesh::Bind() {
		m_VertexArray->Bind();
		m_Shader->Use();
	}

	void Mesh::Unbind() {
		m_VertexArray->Unbind();
	}

	void Mesh::Render() {
		glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetDataSize() / sizeof(unsigned int), GL_UNSIGNED_INT, NULL);
	}

	Shader* Mesh::GetShader() {
		return m_Shader;
	}
}