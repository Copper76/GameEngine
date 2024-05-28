#pragma once
#include "Fengshui/Renderer/RenderShapes/RenderShapeBase.h"

namespace Fengshui
{
	class RenderShapeCube  : public RenderShape
	{
	public:
		explicit RenderShapeCube(const glm::vec3* pts, const int num) {
			Build(pts, num);
			m_Shape = RenderShapeType::SHAPE_CUBE;
		}

		virtual void Build(const glm::vec3* pts, const int num) override;

		virtual RenderShapeType GetType() const override { return RenderShapeType::SHAPE_CUBE; }
	};
}