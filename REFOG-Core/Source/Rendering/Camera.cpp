#include "REFOG/Rendering/Camera.h"
#include "glm/gtc/matrix_transform.hpp"

namespace REFOG {
	std::unique_ptr<Camera> GlobalCamera;

	Camera::Camera() {
		m_Position = glm::vec3(0.0f);
		m_Rotation = glm::vec3(0.0f);
	}

	void Camera::Ortho(glm::vec4 Size) {
		m_Projection = glm::ortho(Size.x, Size.y, Size.z, Size.w);
	}

	void Camera::SetPositionAndRotation(glm::vec3 Position, glm::vec3 Rotation) {
		m_Position = Position;
		m_Rotation = Rotation;
		/*m_View = glm::translate(glm::mat4(1.0f), Position);
		m_View = glm::rotate(m_View, Rotation.x, { 1, 0, 0 });
		m_View = glm::rotate(m_View, Rotation.y, { 0, 1, 0 });
		m_View = glm::rotate(m_View, Rotation.z, { 0, 0, 1 });*/
		//m_View = glm::inverse(m_View);
		/*m_View = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));*/
		m_View = glm::translate(glm::mat4(1.0f), Position) * 
			glm::rotate(glm::mat4(1.0f), Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		m_View = glm::inverse(m_View);
	}

	glm::mat4 Camera::GetView() {
		return m_View;
	}
	glm::mat4 Camera::GetProjection() {
		return m_Projection;
	}
}