#pragma once

#include "Fengshui/ECS/ECS.h"
#include "Fengshui/ECS/Components.h"

namespace Fengshui
{
	class TransformSystem : public System
	{
	public:
		glm::mat4 GetLocalTransformMatrix(EntityID entity);
		static glm::mat4 GetWorldTransformMatrix(EntityID entity);

		glm::mat4 GetLocalTransformMatrix2D(EntityID entity);
		static glm::mat4 GetWorldTransformMatrix2D(EntityID entity);

		static Transform GetWorldTransform(EntityID entity);//This is used so commonly I should make it static
	private:
		static glm::mat4 GetTransformMatrix(EntityID entity);
		glm::mat4 GetTransformMatrix(const Transform transform);
		static glm::mat4 GetTransformMatrix2D(EntityID entity);
	};
}