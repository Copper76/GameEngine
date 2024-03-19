#pragma once

#include "Fengshui/ECS/Components/BaseComponent.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Fengshui
{
	class TransformComponent2D : public Component
	{
	public:
		TransformComponent2D(glm::vec2& position = glm::vec2(0.0f), float rotation = 0.0f, glm::vec2& scale = glm::vec2(1.0f));

		virtual ~TransformComponent2D() = default;

		glm::mat4 GetTransform();

		COMPONENT_CLASS_TYPE(ComponentTransform2D);

	public:
		glm::vec2 Position;
		float Rotation;
		glm::vec2 Scale;
	};
}