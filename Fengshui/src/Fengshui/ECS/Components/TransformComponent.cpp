#include "fspch.h"
#include "TransformComponent.h"

namespace Fengshui
{
	TransformComponent::TransformComponent(glm::vec3& position, glm::vec3& rotation, glm::vec3& scale)
		: Position(position), Rotation(rotation), Scale(scale)
	{
	
	}

	glm::mat4 TransformComponent::GetTransform()
	{
		glm::vec3 rotation = glm::radians(Rotation);
		float rotationAngle = glm::length(rotation);
		return  glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), Position), rotationAngle, rotationAngle == 0.0f ? glm::vec3(1.0) : glm::normalize(rotation)), Scale);
	}
}