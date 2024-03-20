#pragma once

#include "Fengshui/ECS/Components/BaseComponent.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Fengshui
{
	class TransformComponent : public Component
	{
	public:
		TransformComponent(glm::vec3& position = glm::vec3(0.0f), glm::vec3& rotation = glm::vec3(0.0f), glm::vec3& scale = glm::vec3(1.0f));

		virtual ~TransformComponent() = default;

		glm::mat4 GetTransform();

		COMPONENT_CLASS_TYPE(ComponentTransform);

	public:
		glm::vec3 Position;
		glm::vec3 Rotation;
		glm::vec3 Scale;
	};
}

