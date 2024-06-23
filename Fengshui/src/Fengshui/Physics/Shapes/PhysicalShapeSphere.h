//
//	ShapeSphere.h
//
#pragma once
#include "PhysicalShapeBase.h"

namespace Fengshui
{
/*
====================================================
ShapeSphere
====================================================
*/
	class PhysicalShapeSphere : public PhysicalShape {
	public:
		PhysicalShapeSphere() {
			m_Radius = 1.0f;
			m_CenterOfMass = glm::vec3(0.0f);

			m_BaseRadius = 1.0f;
		}

		PhysicalShapeSphere(const float radius) {
			m_Radius = radius;
			m_CenterOfMass = glm::vec3(0.0f);

			m_BaseRadius = radius;
		}

		glm::vec3 Support(const glm::vec3& dir, const Transform& transform, const float bias) const override;

		glm::mat3 InertiaTensor() const override;

		virtual void Rebuild(const glm::vec3 offset, const glm::vec3 scale) override;

		Bounds GetBounds(const Transform& transform) const override;

		ShapeType GetType() const override { return ShapeType::SHAPE_SPHERE; }

		virtual std::string GetDisplayName() const override { return "Sphere Collider"; }

	public:
		float m_Radius;
	private:
		float m_BaseRadius;
	};
}