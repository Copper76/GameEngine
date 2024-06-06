#pragma once
#include "Fengshui/Renderer/RenderShapes/RenderShapeBase.h"

namespace Fengshui
{
	class RenderShapeCube  : public RenderShape
	{
	public:
		explicit RenderShapeCube() = default;

		virtual ShapeType GetType() const override { return ShapeType::SHAPE_CUBE; }

		virtual std::string GetDisplayName() const override { return "Cube"; }
	};
}