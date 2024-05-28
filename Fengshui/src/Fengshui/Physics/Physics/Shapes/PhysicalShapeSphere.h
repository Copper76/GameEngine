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
		/*explicit ShapeSphere( const float radius ) : m_radius( radius ) {
			m_centerOfMass.Zero();
		}*/
		explicit PhysicalShapeSphere(const float radius) : m_radius(radius) {
			m_centerOfMass = glm::vec3(0.0f);
		}

		//Vec3 Support( const Vec3 & dir, const Vec3 & pos, const Quat & orient, const float bias ) const override;
		glm::vec3 Support(const glm::vec3& dir, const Transform transform, const float bias) const override;

		//Mat3 InertiaTensor() const override;
		glm::mat3 InertiaTensor(const Transform transform) const override;

		//Bounds GetBounds( const Vec3 & pos, const Quat & orient ) const override;
		Bounds GetBounds(const Transform transform) const override;
		Bounds GetBounds(const glm::vec3 scale) const override;

		PhysicalShapeType GetType() const override { return SHAPE_SPHERE; }

	public:
		float m_radius;
	};
}