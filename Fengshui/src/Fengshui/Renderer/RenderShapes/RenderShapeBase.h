#pragma once
#include "Fengshui/Maths/Geometry.h"

#include <glm/glm.hpp>

namespace Fengshui
{
	/*
	====================================================
	Shape
	====================================================
	*/
	class RenderShape
	{
	public:
		virtual void Build(const glm::vec3* pts, const int num) {}

		virtual ShapeType GetType() const = 0;

		virtual std::string GetDisplayName() const = 0;

		virtual const std::vector<glm::vec3> GetVertexCoords() const { return std::vector<glm::vec3>(); }
	};
}