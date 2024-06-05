#pragma once
#include "Fengshui/Renderer/RenderShapes/RenderShapeBase.h"

namespace Fengshui
{
	class RenderShapeSphere : public RenderShape
	{
	public:
		explicit RenderShapeSphere(float radius = 1.0f, int divisions = 16);

		void Build(float radius, int divisions);

		void Rebuild();

		virtual ShapeType GetType() const override { return ShapeType::SHAPE_SPHERE; }

		virtual std::string GetDisplayName() const override { return "Sphere"; }

		inline void SetRadius(float radius) { m_Radius = radius; }
		inline void SetDivisions(int divisions) { m_Divisions = divisions; }

		inline const float GetRadius() const { return m_Radius; }
		inline const int GetDivisions() const { return m_Divisions; }
		inline const std::vector<glm::vec3> GetVertexCoords() const { return m_VertexCoords; }
		inline const int GetVertexNum() const { return (int)m_VertexCoords.size(); }

		inline float& GetRadius() { return m_Radius; }
		inline int& GetDivisions() { return m_Divisions; }
	private:
		float m_Radius;
		int m_Divisions;
		std::vector<glm::vec3> m_VertexCoords;
	};
}