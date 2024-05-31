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
			//m_radius = glm::vec3(radius);
			m_radius = 1.0f;
			m_CenterOfMass = glm::vec3(0.0f);
		}

		PhysicalShapeSphere(const float radius) {
			//m_radius = glm::vec3(radius);
			m_radius = radius;
			m_CenterOfMass = glm::vec3(0.0f);
		}

		PhysicalShapeSphere(const glm::vec3 offset, const glm::vec3 scale) {
			//m_radius = scale;
			//m_radius = scale;
			m_CenterOfMass = offset;
		}

		glm::vec3 Support(const glm::vec3& dir, const Transform transform, const float bias) const override;

		glm::mat3 InertiaTensor() const override;

		Bounds GetBounds(const Transform transform) const override;

		ShapeType GetType() const override { return ShapeType::SHAPE_SPHERE; }

		virtual std::string GetDisplayName() const override { return "Sphere Collider"; }

	public:
		float m_radius;
	private:
		glm::vec3 m_BaseRadius;
	};
}