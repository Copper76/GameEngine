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
		PhysicalShapeConvex(const glm::vec3* pts, const int num) {
			Build(pts, num);
		}

		PhysicalShapeConvex(const glm::vec3* pts, const int num, const glm::vec3 offset, const glm::vec3 scale) {
			Build(pts, num, offset, scale);
		}

		void Build(const glm::vec3* pts, const int num) override;

		virtual void Build(const glm::vec3* pts, const int num, const glm::vec3 offset, const glm::vec3 scale) override;

		virtual void Rebuild(const glm::vec3 offset, const glm::vec3 scale) override;

		glm::vec3 Support(const glm::vec3& dir, const Transform& transform, const float bias) const override;

		glm::mat3 InertiaTensor() const override;

		Bounds GetBounds(const Transform& transform) const override;

		float FastestLinearSpeed(const glm::vec3& angularVelocity, const glm::vec3& dir) const override;

		ShapeType GetType() const override { return ShapeType::SHAPE_CONVEX; }

		virtual std::string GetDisplayName() const override { return "Convex Collider"; }

		inline void SetVertexNum(int vertexNum) { m_VertexNum = vertexNum; }

		inline const int GetVertexNum() const { return m_VertexNum; }

		inline const std::vector<glm::vec3> GetBaseCoords() const { return m_BaseCoords; }

	public:
		std::vector<glm::vec3> m_VertexCoords;
		Bounds m_bounds;
		glm::mat3 m_inertiaTensor;
	private:
		std::vector<glm::vec3> m_BaseCoords;
		int m_VertexNum;
	};
}