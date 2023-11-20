#pragma once
#include "Objects/Objects.h"
#include <string_view>
#include <vector>

namespace REFOG {
	enum ShaderFile {
		Custom,
		Default,
		Sprite,
	};

	struct ShaderData {
		ShaderFile File;
		std::vector<const char*> ShaderFilePaths = {}; // Optional
	};

	namespace VertexShaderElements {
		extern std::vector<VertexElement> DefaultShader;
		extern std::vector<VertexElement> SpriteShader;
	}

	class Mesh {
	private:
		VertexArray* m_VertexArray;
		Buffer* m_VertexBuffer;
		Buffer* m_IndexBuffer;
		Shader* m_Shader;

		glm::mat4 m_Model;
	public:
		Mesh();
		virtual ~Mesh();

		void SetupShader(ShaderData);
		void SetupVertexBuffer(void*, GLsizei);
		void SetupIndexBuffer(void*, GLsizei);
		void SetupVertexArray(std::vector<VertexElement>);

		void Bind();
		void Unbind();
		void Render();

		Shader* GetShader();
	};
}