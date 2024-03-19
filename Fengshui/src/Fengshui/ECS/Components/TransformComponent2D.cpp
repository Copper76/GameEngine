#include "fspch.h"
#include "TransformComponent2D.h"

namespace Fengshui
{
	TransformComponent2D::TransformComponent2D(glm::vec2& position, float rotation, glm::vec2& scale)
		: Position(position), Rotation(rotation), Scale(scale)
	{

	}

	glm::mat4 TransformComponent2D::GetTransform()
	{
		return  glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), { Position.x, Position.y, 0.0f }), glm::radians(Rotation), { 0.0f, 0.0f, 1.0f }), { Scale.x, Scale.y, 1.0f });
	}
}