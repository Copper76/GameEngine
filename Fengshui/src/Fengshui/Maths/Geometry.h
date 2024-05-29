#pragma once
#include "Fengshui/Logging/Log.h"

#include "Fengshui/Maths/Triangulation/BowyerWatson.h"
#include "Fengshui/Maths/Triangulation/FurthestPoint.h"

namespace Fengshui
{
	class Geometry
	{
	public:
		static void BuildConvexHull(const std::vector<glm::vec3>& verts, std::vector<glm::vec3>& hullPts, std::vector<Triangle>& hullTris) {
			if (verts.size() < 4) {
				return;
			}

			s_TriMethod->BuildConvexHull(verts, hullPts, hullTris);
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