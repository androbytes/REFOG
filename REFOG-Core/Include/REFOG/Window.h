#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <string_view>

#ifdef _DEBUG
void APIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
#endif

namespace REFOG {
	class Window {
	private:
		GLFWwindow* m_Window;
	public:
		Window(std::string_view name, glm::vec2 scale);
		virtual ~Window();

		GLFWcursor* LoadGLFWCursor(std::string_view, glm::vec2 hotpoint = {0, 0});
		void DestroyCursor(GLFWcursor*);
		void SetCursor(GLFWcursor*);
		void SetupFramebuffer();

		bool IsOpen();
		void SwapBuffer();
		glm::vec2 GetWindowSize();
	};
}