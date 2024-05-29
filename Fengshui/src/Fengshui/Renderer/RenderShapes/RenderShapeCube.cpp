#include "fspch.h"
#include "RenderShapeCube.h"

namespace Fengshui
{
	void RenderShapeCube::Build(const glm::vec3* pts, const int num)
	{
		for (int i = 0; i < num; i++)
		{
			m_VertexCoords.push_back(pts[i]);
		}

		Triangle tri;

		tri.a = 0;
		tri.b = 1;
		tri.c = 2;
		m_Tris.push_back(tri);

		tri.a = 2;
		tri.b = 3;
		tri.c = 0;
		m_Tris.push_back(tri);

		tri.a = 1;
		tri.b = 5;
		tri.c = 6;
		m_Tris.push_back(tri);

		tri.a = 6;
		tri.b = 2;
		tri.c = 1;
		m_Tris.push_back(tri);

		tri.a = 5;
		tri.b = 4;
		tri.c = 7;
		m_Tris.push_back(tri);

		tri.a = 7;
		tri.b = 6;
		tri.c = 5;
		m_Tris.push_back(tri);

		tri.a = 4;
		tri.b = 0;
		tri.c = 3;
		m_Tris.push_back(tri);

		tri.a = 3;
		tri.b = 7;
		tri.c = 4;
		m_Tris.push_back(tri);

		tri.a = 3;
		tri.b = 2;
		tri.c = 6;
		m_Tris.push_back(tri);

		tri.a = 6;
		tri.b = 7;
		tri.c = 3;
		m_Tris.push_back(tri);

		tri.a = 4;
		tri.b = 5;
		tri.c = 1;
		m_Tris.push_back(tri);

		tri.a = 1;
		tri.b = 0;
		tri.c = 4;
		m_Tris.push_back(tri);
	}
}
