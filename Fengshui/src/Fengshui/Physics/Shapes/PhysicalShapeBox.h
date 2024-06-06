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
		PhysicalShapeBox()
		{
			glm::vec3 g_boxUnit[] = {
			glm::vec3(-0.5,-0.5,-0.5),
			glm::vec3(0.5,-0.5,-0.5),
			glm::vec3(0.5, 0.5,-0.5),
			glm::vec3(-0.5, 0.5,-0.5),

			glm::vec3(-0.5,-0.5, 0.5),
			glm::vec3(0.5,-0.5, 0.5),
			glm::vec3(0.5, 0.5, 0.5),
			glm::vec3(-0.5, 0.5, 0.5),
			};
			Build(g_boxUnit, 8);
		}

		PhysicalShapeBox(const glm::vec3 offset, const glm::vec3 scale)
		{
			glm::vec3 g_boxUnit[] = {
			glm::vec3(-0.5,-0.5,-0.5),
			glm::vec3(0.5,-0.5,-0.5),
			glm::vec3(0.5, 0.5,-0.5),
			glm::vec3(-0.5, 0.5,-0.5),

			glm::vec3(-0.5,-0.5, 0.5),
			glm::vec3(0.5,-0.5, 0.5),
			glm::vec3(0.5, 0.5, 0.5),
			glm::vec3(-0.5, 0.5, 0.5),
			};
			Build(g_boxUnit, 8, offset, scale);
		}

		PhysicalShapeBox(const glm::vec3* pts, const int num) 
		{
			Build(pts, num);
		}

		PhysicalShapeBox(const glm::vec3* pts, const int num, const glm::vec3 offset, const glm::vec3 scale) {
			Build(pts, num, offset, scale);
		}

		void Build(const glm::vec3* pts, const int num) override;

		virtual void Build(const glm::vec3* pts, const int num, const glm::vec3 offset, const glm::vec3 scale) override;

		virtual void Rebuild(const glm::vec3 offset, const glm::vec3 scale) override;

		glm::vec3 Support(const glm::vec3& dir, const Transform transform, const float bias) const override;

		glm::mat3 InertiaTensor() const override;

		Bounds GetBounds(const Transform transform) const override;

		float FastestLinearSpeed(const glm::vec3& angularVelocity, const glm::vec3& dir) const override;

		ShapeType GetType() const override { return ShapeType::SHAPE_CUBE; }

		virtual std::string GetDisplayName() const override { return "Box Collider"; }

	public:
		//These are scaled values
		std::vector<glm::vec3> m_VertexCoords;
		Bounds m_bounds;
	private:
		glm::vec3 m_BaseCoords[8];
	};
}