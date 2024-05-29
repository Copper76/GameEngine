#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "Fengshui/Maths/Bounds.h"

namespace Fengshui
{
	struct Edge {
		int a;
		int b;

		Edge()
		{
			a = 0;
			b = 0;
		}

		Edge(int first, int second)
		{
			a = first;
			b = second;
		}

		bool operator == (const Edge& rhs) const {
			return ((a == rhs.a && b == rhs.b) || (a == rhs.b && b == rhs.a));
		}
	};

	struct Triangle
	{
		int a, b, c;

		Triangle()
		{
			a = 0;
			b = 0;
			c = 0;
		}

		Triangle(int first, int second, int third)
		{
			a = first;
			b = second;
			c = third;
		}
	};

	struct Tetrahedron
	{
		int a, b, c, d;

		Tetrahedron(int first, int second, int third, int fourth)
		{
			a = first;
			b = second;
			c = third;
			d = fourth;
		}

		Tetrahedron(Triangle& tri, int last)
		{
			a = tri.a;
			b = tri.b;
			c = tri.c;
			d = last;
		}
	};

	class TriMethod
	{
	public:
		glm::vec3 CalculateCenterOfMassTetrahedron(const std::vector<glm::vec3>& pts, const std::vector<Triangle>& tris);

		glm::mat3 CalculateInertiaTensorTetrahedron(const std::vector<glm::vec3>& pts, const std::vector<Triangle>& tris, const glm::vec3& cm);

		virtual void BuildConvexHull(std::vector<glm::vec3>& verts, std::vector< Triangle >& hullTris) = 0;
	};
}