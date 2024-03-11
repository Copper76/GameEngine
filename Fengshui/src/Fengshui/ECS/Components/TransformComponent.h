#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Fengshui/ECS/Components/BaseComponent.h"

namespace Fengshui
{
	class TransformComponent : public Component
	{
	public:
		TransformComponent(uint32_t id, glm::vec3& position = glm::vec3(0.0f), glm::vec3& rotation = glm::vec3(0.0f), glm::vec3& scale = glm::vec3(1.0f)) : Component(id), Position(position), Rotation(rotation), Scale(scale)
		{
		}

		glm::mat4 GetTransform()
		{ 
			glm::vec3 rotation = glm::radians(Rotation);
			float rotationAngle = glm::length(rotation);
			return  glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), Position), rotationAngle, rotationAngle == 0.0f ? glm::vec3(1.0) : glm::normalize(rotation)), Scale);
		}
		//glm::mat4 GetTransform() { return  glm::scale(glm::translate(glm::mat4(1.0f), Position), Scale); }

		COMPONENT_CLASS_TYPE(ComponentTransform);

	public:
		glm::vec3 Position;
		glm::vec3 Rotation;
		glm::vec3 Scale;
	};
}

