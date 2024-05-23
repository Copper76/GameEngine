#pragma once

#include "Fengshui/ECS/ECS.h"
#include "Fengshui/ECS/Components.h"

namespace Fengshui
{
	class TransformSystem : public System
	{
	public:
		glm::mat4 GetLocalTransformMatrix(EntityID entity);
		glm::mat4 GetWorldTransformMatrix(EntityID entity);
		Transform GetWorldTransform(EntityID entity);
	private:
		glm::mat4 GetTransformMatrix(EntityID entity);
	};
}