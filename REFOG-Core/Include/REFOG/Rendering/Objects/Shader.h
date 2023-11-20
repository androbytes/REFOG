#pragma once
#include "glm/glm.hpp"
#include "glad/glad.h"
#include <string_view>
#include <unordered_map>

namespace REFOG {
	enum ShaderType {
		Vertex,
		Fragment,
		Geometry
	};

	class Shader {
	private:
		GLuint m_Program;
		std::vector<std::vector<GLuint>> m_Shaders;

		std::unordered_map<std::string_view, GLint> m_UniformMap;

		GLint GetUniformLocation(std::string_view);
	public:
		Shader();
		virtual ~Shader();

		void Use();
		void LoadShader(const char*, ShaderType);
		void LinkProgram();

		void SetBool(const std::string_view&, bool);
		void SetInt(const std::string_view&, int);
		void SetFloat(const std::string_view&, float);
		void SetVec2(const std::string_view&, glm::vec2);
		void SetVec3(const std::string_view&, glm::vec3);
		void SetVec4(const std::string_view&, glm::vec4);
		void SetMat3(const std::string_view&, glm::mat3);
		void SetMat4(const std::string_view&, glm::mat4);
	};
}