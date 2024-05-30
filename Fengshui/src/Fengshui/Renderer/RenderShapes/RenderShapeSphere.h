#pragma once
#include "Fengshui/Renderer/RenderShapes/RenderShapeBase.h"

namespace Fengshui
{
	class RenderShapeSphere : public RenderShape
	{
	public:
		explicit RenderShapeSphere(float radius = 1.0f, int divisions = 32);

		virtual ShapeType GetType() const override { return ShapeType::SHAPE_SPHERE; }

		virtual std::string GetDisplayName() const override { return "Sphere"; }

		inline void SetRadius(float radius) { m_Radius = radius; }
		inline void SetDivisions(int divisions) { m_Divisions = divisions; }

		inline const float GetRadius() const { return m_Radius; }
		inline const int GetDivisions() const { return m_Divisions; }

		inline float& GetRadius() { return m_Radius; }
		inline int& GetDivisions() { return m_Divisions; }
	private:
		float m_Radius;
		int m_Divisions;
	};
}