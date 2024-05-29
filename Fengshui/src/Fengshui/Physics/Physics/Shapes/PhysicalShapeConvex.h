//
//	ShapeConvex.h
//
#pragma once
#include "PhysicalShapeBase.h"

#include "Fengshui/Core/Geometry.h"

namespace Fengshui
{
	/*
	====================================================
	ShapeConvex
	====================================================
	*/
	class PhysicalShapeConvex : public PhysicalShape {
	public:
		/*explicit ShapeConvex( const Vec3 * pts, const int num ) {
			Build( pts, num );
		}*/
		explicit PhysicalShapeConvex(const glm::vec3* pts, const int num) {
			Build(pts, num);
		}

		explicit PhysicalShapeConvex(const std::vector<glm::vec3> pts, const std::vector<Triangle> tris) {
			Build(pts, tris);
		}

		//void Build( const Vec3 * pts, const int num ) override;
		void Build(const glm::vec3* pts, const int num) override;

		void Build(const std::vector<glm::vec3> pts, const std::vector<Triangle> tris);

		//Vec3 Support( const Vec3 & dir, const Vec3 & pos, const Quat & orient, const float bias ) const override;
		glm::vec3 Support(const glm::vec3& dir, const Transform transform, const float bias) const override;

		//Mat3 InertiaTensor() const override { return m_inertiaTensor; }
		glm::mat3 InertiaTensor(const Transform transform) const override;

		//Bounds GetBounds( const Vec3 & pos, const Quat & orient ) const override;
		Bounds GetBounds(const Transform transform) const override;
		Bounds GetBounds(const glm::vec3 scale) const override;

		//float FastestLinearSpeed( const Vec3 & angularVelocity, const Vec3 & dir ) const override;
		float FastestLinearSpeed(const glm::vec3& angularVelocity, const glm::vec3& dir) const override;

		PhysicalShapeType GetType() const override { return SHAPE_CONVEX; }

	public:
		std::vector<glm::vec3> m_points;
		Bounds m_bounds;
		glm::mat3 m_inertiaTensor;
	};
}