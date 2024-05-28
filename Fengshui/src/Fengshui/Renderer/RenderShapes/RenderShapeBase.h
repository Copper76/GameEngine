#pragma once
#include "Fengshui/Core/Geometry.h"

#include <glm/glm.hpp>

namespace Fengshui
{
	enum class RenderShapeType {
		SHAPE_SPHERE,
		SHAPE_CUBE,
		SHAPE_CONVEX,
	};

	struct Transform;
	/*
	====================================================
	Shape
	====================================================
	*/
	class RenderShape
	{
	public:
		virtual void Build(const glm::vec3* pts, const int num) {}

		virtual RenderShapeType GetType() const = 0;

		inline const std::vector<glm::vec3> GetVertexCoords() const { return m_VertexCoords; }
		inline const std::vector<Triangle> GetTris() const { return m_Tris; }

	protected:
		std::vector<glm::vec3> m_VertexCoords;
		std::vector<Triangle> m_Tris;
	};
}