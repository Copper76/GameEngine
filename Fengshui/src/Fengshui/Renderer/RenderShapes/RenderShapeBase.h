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

		virtual std::string GetDisplayName() const { return "Shape"; }
	};
}