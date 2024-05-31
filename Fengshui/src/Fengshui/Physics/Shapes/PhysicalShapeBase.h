//
//	ShapeBase.h
//
#pragma once
#include "Fengshui/Renderer//RenderShapes/RenderShapes.h"

#include "Fengshui/Maths/Vector.h"
#include "Fengshui/Maths/Matrix.h"
#include <vector>

namespace Fengshui
{
	struct Transform;
	struct Collider;
	/*
	====================================================
	Shape
	====================================================
	*/
	class PhysicalShape : public RenderShape
	{
	public:
		virtual glm::vec3 Support(const glm::vec3& dir, const Transform transform, const float bias) const = 0;

		virtual void Build(const glm::vec3* pts, const int num, const glm::vec3 offset, const glm::vec3 scale) {};

		virtual void Rebuild(const glm::vec3 offset, const glm::vec3 scale) {};

		virtual glm::mat3 InertiaTensor() const = 0;

		virtual Bounds GetBounds(const Transform transform) const = 0;

		virtual glm::vec3 GetCenterOfMass() const { return m_CenterOfMass; }

		virtual float FastestLinearSpeed(const glm::vec3& angularVelocity, const glm::vec3& dir) const { return 0.0f; }

	protected:
		glm::vec3 m_CenterOfMass;
	};
}
