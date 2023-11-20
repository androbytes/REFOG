#pragma once
#include "glm/glm.hpp"
#include <memory>

namespace REFOG {
	class Camera {
	private:
		glm::mat4 m_View       = glm::mat4(1.0f);
		glm::mat4 m_Projection = glm::mat4(1.0f);

		/* These will be used to set certain parts
		 * of the view matrix.
		 */
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
	public:
		Camera();

		/* @param
		 * Left, Right, Bottom and Top of ortho camera
		 */
		void Ortho(glm::vec4);
		//void Perspective();

		void SetPositionAndRotation(glm::vec3, glm::vec3);

		glm::mat4 GetView();
		glm::mat4 GetProjection();
	};

	extern std::unique_ptr<Camera> GlobalCamera;
}