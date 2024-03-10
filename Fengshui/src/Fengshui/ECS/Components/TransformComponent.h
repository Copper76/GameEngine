#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Fengshui/ECS/Components/BaseComponent.h"

namespace Fengshui
{
	class TransformComponent : public Component
	{
	public:
		TransformComponent(uint32_t id, glm::vec3& position = glm::vec3(0.0f), glm::vec3& rotation = glm::vec3(0.0f), glm::vec3& scale = glm::vec3(1.0f)) : Component(id), m_Position(position), m_Rotation(rotation), m_Scale(scale)
		{

		}

		glm::mat4 GetTransform() { return  glm::scale(glm::translate(glm::mat4(1.0f), m_Position), m_Scale); }

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Scale;
		glm::vec3 m_Rotation;
	};
}

