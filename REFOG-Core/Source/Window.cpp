#include "REFOG/Window.h"
#include "REFOG/Logger.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifdef _DEBUG
void APIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	if (severity == GL_DEBUG_SEVERITY_HIGH) {
		REFOG_CRITICAL("OpenGL Error!\n" + std::string(message));
		REFOG_CRITICAL("Source: " + std::to_string(source));
	}
	else if (severity == GL_DEBUG_SEVERITY_MEDIUM) {
		REFOG_ERROR("OpenGL Error!\n" + std::string(message));
		REFOG_ERROR("Source: " + std::to_string(source));
	}
	else if (severity == GL_DEBUG_SEVERITY_LOW) {
		REFOG_WARN("OpenGL Error!\n" + std::string(message));
		REFOG_WARN("Source: " + std::to_string(source));
	}
#ifdef SHOW_NOTIFICATIONS
	else {
		REFOG_INFO("OpenGL Notification!\n" + std::string(message));
		REFOG_INFO("Source: " + std::to_string(source));
	}
#endif
}
#endif

namespace REFOG {
	Window::Window(std::string_view name, glm::vec2 scale) {
		static bool GLFWInitialized = false;
		if (GLFWInitialized == false) {
			assert(glfwInit() && "Failed to init GLFW");
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		REFOG_INFO("Using OpenGL 4.6");

#ifdef _DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
		REFOG_INFO("Using OpenGL Debug Context");
#endif

		m_Window = glfwCreateWindow(scale.x, scale.y, name.data(), nullptr, nullptr);
		assert(m_Window && "Failed to create window");
		REFOG_INFO("Created GLFW Window");

		glfwMakeContextCurrent(m_Window);
		assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) && "Failed to load OpenGL");
		REFOG_INFO("Loaded OpenGL");

#ifdef _DEBUG
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(MessageCallback, 0);
#endif
	}

	Window::~Window() {
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	GLFWcursor* Window::LoadGLFWCursor(std::string_view path, glm::vec2 hotpoint) {
		int x, y, channels;
		unsigned char* pixels = stbi_load(path.data(), &x, &y, &channels, 4);

		GLFWimage* image = new GLFWimage();
		image->width = x;
		image->height = y;
		image->pixels = pixels;

		return glfwCreateCursor(image, hotpoint.x, hotpoint.y);
	}

	void Window::DestroyCursor(GLFWcursor* cursor) {
		glfwDestroyCursor(cursor);
	}

	void Window::SetCursor(GLFWcursor* cursor) {
		glfwSetCursor(m_Window, cursor);
	}

	void Window::SetupFramebuffer() {
		glfwPollEvents(); // has nothing to do with setting up a framebuffer but oh well

		glm::vec2 viewportSize = GetWindowSize();
		glViewport(0, 0, viewportSize.x, viewportSize.y);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	bool Window::IsOpen() {
		return !glfwWindowShouldClose(m_Window);
	}

	void Window::SwapBuffer() {
		glfwSwapBuffers(m_Window);
	}

	glm::vec2 Window::GetWindowSize() {
		int x, y;
		glfwGetWindowSize(m_Window, &x, &y);
		return {x, y};
	}
}