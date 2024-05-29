//
//	ShapeBase.h
//
#pragma once
#include "Fengshui/Maths/Geometry.h"
#include "Fengshui/Renderer//RenderShapes/RenderShapes.h"

#include "Fengshui/Maths/Vector.h"
#include "Fengshui/Maths/Matrix.h"
#include <vector>

namespace Fengshui
{
	struct Transform;
	/*
	====================================================
	Shape
	====================================================
	*/
	class PhysicalShape
	{
	public:
		virtual void Build(const glm::vec3* pts, const int num) {}

		virtual glm::vec3 Support(const glm::vec3& dir, const Transform transform, const float bias) const = 0;

		virtual glm::mat3 InertiaTensor(const Transform transform) const = 0;

		virtual Bounds GetBounds(const Transform transform) const = 0;
		virtual Bounds GetBounds(const glm::vec3 scale) const = 0;

		virtual glm::vec3 GetCenterOfMass(const glm::vec3 offset, const glm::vec3 scale) const { return (m_centerOfMass * scale) + offset; }

		enum PhysicalShapeType {
			SHAPE_SPHERE,
			SHAPE_BOX,
			SHAPE_CONVEX,
		};
		virtual PhysicalShapeType GetType() const = 0;

		virtual float FastestLinearSpeed(const glm::vec3& angularVelocity, const glm::vec3& dir) const { return 0.0f; }

	protected:
		glm::vec3 m_centerOfMass;
	};
}
