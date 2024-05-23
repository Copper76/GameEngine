//
//	ShapeBox.h
//
#pragma once
#include "ShapeBase.h"

/*
====================================================
ShapeBox
====================================================
*/
namespace Fengshui
{
	class ShapeBox : public Shape {
	public:
		/*explicit ShapeBox( const Vec3 * pts, const int num ) {
			Build( pts, num );
		}*/

		explicit ShapeBox(const glm::vec3* pts, const int num) {
			Build(pts, num);
		}
		//void Build( const Vec3* pts, const int num ) override;
		void Build(const glm::vec3* pts, const int num) override;

		//Vec3 Support( const Vec3& dir, const Vec3& pos, const Quat & orient, const float bias ) const override;
		glm::vec3 Support(const glm::vec3& dir, const Transform transform, const float bias) const override;

		//Mat3 InertiaTensor() const override;
		glm::mat3 InertiaTensor(const Transform transform) const override;

		//Bounds GetBounds( const Vec3 & pos, const Quat & orient ) const override;
		Bounds GetBounds(const Transform transform) const override;
		Bounds GetBounds(const glm::vec3 scale) const override;

		//float FastestLinearSpeed( const Vec3 & angularVelocity, const Vec3 & dir ) const override;
		float FastestLinearSpeed(const glm::vec3& angularVelocity, const glm::vec3& dir) const override;

		shapeType_t GetType() const override { return SHAPE_BOX; }

	public:
		//std::vector< Vec3 > m_points;
		std::vector<glm::vec3> m_points;
		Bounds m_bounds;
	};
}