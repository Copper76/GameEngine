#include "fspch.h"
#include "BowyerWatson.h"
#include "Fengshui/PhysicsCustom/Maths/Bounds.h"

namespace Fengshui
{
	//The class is currently under development and is incomplete
	void BuildSupraTetrahedron(std::vector<glm::vec3>& verts, std::vector<Tetrahedron>& hullTetra)
	{
		hullTetra.clear();

		//Use a bound to find the maximum and minimum of axis
		Bounds bound;

		int vertSize = (int)verts.size();

		bound.Expand(&Vec3(verts.data()), vertSize);
		float vertScale = 10.0f;

		float dx = bound.WidthX() * vertScale;
		float dy = bound.WidthY() * vertScale;
		float dz = bound.WidthY() * vertScale;

		float minX = bound.mins.x - dx;
		float maxX = bound.maxs.x - dx;
		float minY = bound.mins.y - dx;
		float maxY = bound.maxs.y - dx;
		float minZ = bound.mins.z - dx;
		float maxZ = bound.maxs.z - dx;

		verts.push_back(glm::vec3(minX, minY, minZ));
		verts.push_back(glm::vec3(maxX, minY, minZ));
		verts.push_back(glm::vec3(minX, maxY, minZ));
		verts.push_back(glm::vec3(minX, minY, maxZ));

		hullTetra.push_back(Tetrahedron(vertSize + 0, vertSize + 1, vertSize + 2, vertSize = 3));
	}

	void ExpandHull(std::vector<glm::vec3>& verts, std::vector<Tetrahedron>& hullTetra)
	{
		//for (int i = 0; i < verts.size() - 4; i++)
		//{
		//	glm::vec3 newPt = verts[i];
		//	
		//	for (Tetrahedron tetra : hullTetra)
		//	{

		//	}
		//}
	}

	void BowyerWatson::BuildConvexHull(std::vector<glm::vec3>& verts, std::vector<Triangle>& hullTris, bool removeUnused)
	{
		std::vector<Tetrahedron>& hullTetra = std::vector<Tetrahedron>();
		BuildSupraTetrahedron(verts, hullTetra);

		ExpandHull(verts, hullTetra);
	}
}