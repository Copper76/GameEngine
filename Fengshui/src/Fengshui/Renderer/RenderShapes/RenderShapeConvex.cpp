#include "fspch.h"
#include "RenderShapeConvex.h"

#include "Fengshui/Maths/Geometry.h"

namespace Fengshui
{
	void RenderShapeConvex::Build(const glm::vec3* pts, const int num)
	{
		m_VertexCoords.clear();
		m_VertexCoords.reserve(num);
		for (int i = 0; i < num; i++) {
			m_VertexCoords.push_back(pts[i]);
		}

		//Create the hull
		std::vector<glm::vec3> hullPoints;
		Geometry::BuildConvexHull(m_VertexCoords, hullPoints, m_Tris);
		m_VertexCoords = hullPoints;
	}
}