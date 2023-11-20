#include "REFOG/Rendering/Objects/Shader.h"
#include "glm/gtc/type_ptr.hpp"
#include "REFOG/Logger.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

namespace REFOG {
	GLint Shader::GetUniformLocation(std::string_view name) {
		auto mapIndex = m_UniformMap.find(name);
		GLint index;
		if (mapIndex == m_UniformMap.end()) {
			m_UniformMap[name] = glGetUniformLocation(m_Program, name.data());
			index = m_UniformMap[name];
		}
		else {
			index = mapIndex->second;
		}
		return index;
	}

	Shader::Shader() {
		m_Program = glCreateProgram();
		m_Shaders = std::vector<std::vector<GLuint>>();
		m_Shaders.resize(3);
	}

	Shader::~Shader() {
		glDeleteProgram(m_Program);
	}

	void Shader::Use() {
		glUseProgram(m_Program);
	}

	void Shader::LoadShader(const char* fileName, ShaderType shaderType) {
		std::ifstream shaderFileStream;
		shaderFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		std::stringstream shaderStream;
		std::string shaderData;
		try {
			shaderFileStream.open(std::filesystem::current_path().concat(fileName).c_str());
			shaderStream << shaderFileStream.rdbuf();
			shaderFileStream.close();
		}
		catch (std::ifstream::failure failure) {
			REFOG_ERROR("Failed to load shader file: " + std::string(failure.what()));
		}
		shaderData = shaderStream.str();

		GLuint shader = 0;
		if (shaderType == Vertex)
			shader = glCreateShader(GL_VERTEX_SHADER);
		else if (shaderType == Fragment)
			shader = glCreateShader(GL_FRAGMENT_SHADER);
		else if (shaderType == Geometry)
			shader = glCreateShader(GL_GEOMETRY_SHADER);

		const char* shaderDatacstr = shaderData.c_str();
		glShaderSource(shader, 1, &shaderDatacstr, nullptr);
		glCompileShader(shader);

		int success = 0;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			REFOG_ERROR("Shader compilation failed:\n TYPE: " + std::to_string(shaderType) + "\n INFO: " + std::string(infoLog));
		}

		m_Shaders[shaderType].push_back(shader);
	}

	void Shader::LinkProgram() {
		for (std::vector<GLuint> shaders : m_Shaders) {
			for (GLuint shader : shaders) {
				if(shader != 0)
					glAttachShader(m_Program, shader);
			}
		}

		glLinkProgram(m_Program);

		int success = 0;
		char infoLog[512];
		glGetProgramiv(m_Program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(m_Program, 512, NULL, infoLog);
			REFOG_ERROR("Failed to link program: \n INFO: " + std::string(infoLog));
		}

		for (std::vector<GLuint> shaders : m_Shaders) {
			for (GLuint shader : shaders) {
				if (shader != 0)
					glDetachShader(m_Program, shader);
			}
		}
	}

	void Shader::SetBool(const std::string_view& name, bool value) {
		glProgramUniform1i(m_Program, GetUniformLocation(name), (int)value);
	}
	void Shader::SetInt(const std::string_view& name, int value) {
		glProgramUniform1i(m_Program, GetUniformLocation(name), value);
	}
	void Shader::SetFloat(const std::string_view& name, float value) {
		glProgramUniform1f(m_Program, GetUniformLocation(name), value);
	}
	void Shader::SetVec2(const std::string_view& name, glm::vec2 value) {
		glProgramUniform2f(m_Program, GetUniformLocation(name), value.x, value.y);
	}
	void Shader::SetVec3(const std::string_view& name, glm::vec3 value) {
		glProgramUniform3f(m_Program, GetUniformLocation(name), value.x, value.y, value.z);
	}
	void Shader::SetVec4(const std::string_view& name, glm::vec4 value) {
		glProgramUniform4f(m_Program, GetUniformLocation(name), value.x, value.y, value.z, value.w);
	}
	void Shader::SetMat3(const std::string_view& name, glm::mat3 value) {
		glProgramUniformMatrix3fv(m_Program, GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
	}
	void Shader::SetMat4(const std::string_view& name, glm::mat4 value) {
		glProgramUniformMatrix4fv(m_Program, GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
	}
}