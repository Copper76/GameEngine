#pragma once
#include "Fengshui/Renderer/RenderShapes/RenderShapeBase.h"

namespace Fengshui
{
	class RenderShapeConvex : public RenderShape
	{
	public:
		explicit RenderShapeConvex(const glm::vec3* pts, const int num) {
			Build(pts, num);
			m_Shape = RenderShapeType::SHAPE_CONVEX;
		}

		virtual void Build(const glm::vec3* pts, const int num) override;

		virtual RenderShapeType GetType() const override { return RenderShapeType::SHAPE_CONVEX; }
	};
}