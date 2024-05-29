#pragma once
#include "Fengshui/Logging/Log.h"

#include "Fengshui/Maths/Triangulation/BowyerWatson.h"
#include "Fengshui/Maths/Triangulation/FurthestPoint.h"

namespace Fengshui
{
	enum class ShapeType {
		SHAPE_SPHERE,
		SHAPE_CUBE,
		SHAPE_CONVEX,
	};

	class Geometry
	{
	public:
		static void BuildConvexHull(std::vector<glm::vec3>& verts, std::vector<Triangle>& hullTris) {
			if (verts.size() < 4) {
				return;
			}

			s_TriMethod->BuildConvexHull(verts, hullTris);
		}

		static glm::mat3 CalculateInertiaTensorTetrahedron(const std::vector<glm::vec3>& pts, const std::vector<Triangle>& tris, const glm::vec3& cm)
		{
			return s_TriMethod->CalculateInertiaTensorTetrahedron(pts, tris, cm);
		}

		static glm::vec3 CalculateCenterOfMassTetrahedron(const std::vector<glm::vec3>& pts, const std::vector<Triangle>& tris)
		{
			return s_TriMethod->CalculateCenterOfMassTetrahedron(pts, tris);
		}
	private:
		static TriMethod* s_TriMethod;
	};
}