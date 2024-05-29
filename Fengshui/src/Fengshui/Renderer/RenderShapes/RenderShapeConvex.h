#pragma once
#include "Fengshui/Renderer/RenderShapes/RenderShapeBase.h"

namespace Fengshui
{
	class RenderShapeConvex : public RenderShape
	{
	public:
		explicit RenderShapeConvex(const glm::vec3* pts, const int num) {
			Build(pts, num);
		}

		virtual void Build(const glm::vec3* pts, const int num) override;

		virtual ShapeType GetType() const override { return ShapeType::SHAPE_CONVEX;  }
	};
}