#include "fspch.h"

#include "Fengshui/Renderer/Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Fengshui
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float aspectratio) : m_ProjectionMatrix(glm::ortho(left * aspectratio, right * aspectratio, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix; //order based on opengl
	}
}