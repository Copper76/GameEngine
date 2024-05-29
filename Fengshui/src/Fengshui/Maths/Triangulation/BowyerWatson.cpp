#include "fspch.h"
#include "BowyerWatson.h"

namespace Fengshui
{
	void BowyerWatson::BuildConvexHull(const std::vector<glm::vec3>& verts, std::vector<glm::vec3>& hullPts, std::vector<Triangle>& hullTris)
	{
		//BuildTetrahedron(verts.data(), (int)verts.size(), hullPts, hullTris);

		//ExpandConverHull(hullPts, hullTris, verts);
	}
}