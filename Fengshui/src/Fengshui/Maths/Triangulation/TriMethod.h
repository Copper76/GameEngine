#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "Fengshui/Maths/Bounds.h"

namespace Fengshui
{
	struct Triangle
	{
		int a, b, c;
	};

	struct Edge {
		int a;
		int b;

		bool operator == (const Edge& rhs) const {
			return ((a == rhs.a && b == rhs.b) || (a == rhs.b && b == rhs.a));
		}
	};

	class TriMethod
	{
	public:
		glm::vec3 CalculateCenterOfMassTetrahedron(const std::vector<glm::vec3>& pts, const std::vector<Triangle>& tris);

		glm::mat3 CalculateInertiaTensorTetrahedron(const std::vector<glm::vec3>& pts, const std::vector<Triangle>& tris, const glm::vec3& cm);

		virtual void BuildConvexHull(const std::vector< glm::vec3 >& verts, std::vector< glm::vec3 >& hullPts, std::vector< Triangle >& hullTris) = 0;
	};
}