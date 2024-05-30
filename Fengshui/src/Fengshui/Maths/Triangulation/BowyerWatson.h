#pragma once

#include "Fengshui/Maths/Triangulation/TriMethod.h"

namespace Fengshui
{
	class BowyerWatson : public TriMethod
	{
	public:
		void BuildConvexHull(std::vector<glm::vec3>& verts, std::vector<Triangle>& hullTris, bool removeUnused) override;
	};
}