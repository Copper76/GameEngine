#include "fspch.h"
#include "TransformComponent2D.h"

namespace Fengshui
{
	TransformComponent2D::TransformComponent2D(glm::vec2& position, float depth, float rotation, glm::vec2& scale)
		: Position(position),Depth(depth), Rotation(rotation), Scale(scale)
	{

	}

	glm::mat4 TransformComponent2D::GetTransform()
	{
		if (Rotation == 0.0f)
		{
			return  glm::scale(glm::translate(glm::mat4(1.0f), { Position.x, Position.y, Depth }), { Scale.x, Scale.y, 1.0f });
		}
		else
		{
			return  glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), { Position.x, Position.y, Depth }), glm::radians(Rotation), { 0.0f, 0.0f, 1.0f }), { Scale.x, Scale.y, 1.0f });
		}
	}
}