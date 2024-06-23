#pragma once
#include "Fengshui/Renderer/RenderShapes/RenderShapeBase.h"

namespace Fengshui
{
	class RenderShapeCube  : public RenderShape
	{
	public:
		RenderShapeCube();

		virtual ShapeType GetType() const override { return ShapeType::SHAPE_CUBE; }

		virtual std::string GetDisplayName() const override { return "Cube"; }

		inline const std::vector<glm::vec3> GetVertexCoords() const override { return m_VertexCoords; }

	private:
		std::vector<glm::vec3> m_VertexCoords;
	};
}