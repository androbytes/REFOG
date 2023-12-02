#pragma once
#include "REFOG/Vulkan/Utils.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <string_view>

#ifdef _DEBUG
void APIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
#endif

namespace REFOG {
	namespace Vulkan { extern class Surface; };
	class Window {
	private:
		friend class Vulkan::Surface;

		GLFWwindow* m_Window;
	public:
		Window(std::string_view name, glm::vec2 scale, bool OpenGL = true);
		virtual ~Window();

		GLFWcursor* LoadGLFWCursor(std::string_view, glm::vec2 hotpoint = {0, 0});
		void DestroyCursor(GLFWcursor*);
		void SetCursor(GLFWcursor*);
		void SetupFramebuffer();

		bool IsOpen();
		void SwapBuffer();
		glm::vec2 GetWindowSize();

		void CreateSurface(VkInstance, VkSurfaceKHR*);
	};
}