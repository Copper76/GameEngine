//
//	ShapeBox.h
//
#pragma once
#include "PhysicalShapeBase.h"

/*
====================================================
ShapeBox
====================================================
*/
namespace Fengshui
{
	class PhysicalShapeBox : public PhysicalShape {
	public:
		explicit PhysicalShapeBox(const glm::vec3* pts, const int num) 
		{
			Build(pts, num);
		}

		void Build(const glm::vec3* pts, const int num) override;

		glm::vec3 Support(const glm::vec3& dir, const Transform transform, const float bias) const override;

		glm::mat3 InertiaTensor(const Transform transform) const override;

		Bounds GetBounds(const Transform transform) const override;
		Bounds GetBounds(const glm::vec3 scale) const override;

		float FastestLinearSpeed(const glm::vec3& angularVelocity, const glm::vec3& dir) const override;

		PhysicalShapeType GetType() const override { return SHAPE_BOX; }

	public:
		std::vector<glm::vec3> m_points;
		Bounds m_bounds;
	};
}