//
//	ShapeConvex.h
//
#pragma once
#include "PhysicalShapeBase.h"

#include "Fengshui/Renderer/RenderShapes/RenderShapes.h"

namespace Fengshui
{
	/*
	====================================================
	ShapeConvex
	====================================================
	*/
	class PhysicalShapeConvex : public PhysicalShape {
	public:
		explicit PhysicalShapeConvex(const glm::vec3* pts, const int num) {
			Build(pts, num);
		}

		explicit PhysicalShapeConvex(const std::vector<glm::vec3> pts, const std::vector<Triangle> tris) {
			Build(pts, tris);
		}

		explicit PhysicalShapeConvex(const RenderShape* shape) {
			Build(shape->GetVertexCoords(), shape->GetTris());
		}

		void Build(const glm::vec3* pts, const int num) override;

		void Build(const std::vector<glm::vec3> pts, const std::vector<Triangle> tris);

		glm::vec3 Support(const glm::vec3& dir, const Transform transform, const float bias) const override;

		glm::mat3 InertiaTensor(const Transform transform) const override;

		Bounds GetBounds(const Transform transform) const override;
		Bounds GetBounds(const glm::vec3 scale) const override;

		float FastestLinearSpeed(const glm::vec3& angularVelocity, const glm::vec3& dir) const override;

		ShapeType GetType() const override { return ShapeType::SHAPE_CONVEX; }

	public:
		Bounds m_bounds;
		glm::mat3 m_inertiaTensor;
	};
}