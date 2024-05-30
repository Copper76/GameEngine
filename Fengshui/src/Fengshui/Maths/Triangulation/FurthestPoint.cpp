#include "fspch.h"
#include "FurthestPoint.h"

#include <glm/gtx/norm.hpp>

namespace Fengshui
{
	int FindPointFurthestInDir(const glm::vec3* pts, const int num, const glm::vec3& dir) {
		int maxIdx = 0;
		float maxDist = glm::dot(dir, pts[0]);
		for (int i = 1; i < num; i++) {
			float dist = glm::dot(dir, pts[i]);
			if (dist > maxDist) {
				maxDist = dist;
				maxIdx = i;
			}
		}
		return maxIdx;
	}

	float DistanceFromLine(const glm::vec3& a, const glm::vec3& b, const glm::vec3& pt) {
		glm::vec3 ab = glm::normalize(b - a);//the line to calculate from

		glm::vec3 ray = pt - a; //displacement of point from a
		glm::vec3 projection = ab * glm::dot(ray, ab);//The component of ray that is parallel to ab
		return glm::length(ray - projection);//get the magnitude of the perpendicular component
	}

	glm::vec3 FindPointFurthestFromLine(const glm::vec3* pts, const int num, const glm::vec3& ptA, const glm::vec3& ptB) {
		int maxIdx = 0;
		float maxDist = DistanceFromLine(ptA, ptB, pts[0]);
		for (int i = 1; i < num; i++) {
			float dist = DistanceFromLine(ptA, ptB, pts[i]);
			if (dist > maxDist) {
				maxDist = dist;
				maxIdx = i;
			}
		}
		return pts[maxIdx];
	}

	float DistanceFromTriangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& pt) {
		glm::vec3 ab = b - a;
		glm::vec3 ac = c - a;
		glm::vec3 normal = glm::normalize(glm::cross(ab, ac));

		glm::vec3 ray = pt - a; //displacement of point from a
		return glm::dot(ray, normal);//get the magnitude of the ray in the direction of normal (not necessarily positive)
	}

	glm::vec3 FindPointFurthestFromTriangle(const glm::vec3* pts, const int num, const glm::vec3& ptA, const glm::vec3& ptB, const glm::vec3& ptC) {
		int maxIdx = 0;
		float maxDist = DistanceFromTriangle(ptA, ptB, ptC, pts[0]);
		for (int i = 1; i < num; i++) {
			float dist = DistanceFromTriangle(ptA, ptB, ptC, pts[i]);
			if (dist * dist > maxDist * maxDist) {
				maxDist = dist;
				maxIdx = i;
			}
		}
		return pts[maxIdx];
	}

	void BuildTetrahedron(const glm::vec3* verts, const int num, std::vector<glm::vec3>& hullPts, std::vector<Triangle>& hullTris) {
		hullPts.clear();
		hullTris.clear();

		glm::vec3 points[4];

		int idx = FindPointFurthestInDir(verts, num, glm::vec3(1, 0, 0));
		points[0] = verts[idx];
		idx = FindPointFurthestInDir(verts, num, points[0] * -1.0f);
		points[1] = verts[idx];
		points[2] = FindPointFurthestFromLine(verts, num, points[0], points[1]);
		points[3] = FindPointFurthestFromTriangle(verts, num, points[0], points[1], points[2]);

		//need to make sure the points are in counter clockwise order
		float dist = DistanceFromTriangle(points[0], points[1], points[2], points[3]);
		if (dist > 0.0f) {
			std::swap(points[0], points[1]);
		}

		hullPts.push_back(points[0]);
		hullPts.push_back(points[1]);
		hullPts.push_back(points[2]);
		hullPts.push_back(points[3]);

		Triangle tri;
		tri.a = 0;
		tri.b = 1;
		tri.c = 2;

		hullTris.push_back(tri);

		tri.a = 0;
		tri.b = 2;
		tri.c = 3;

		hullTris.push_back(tri);

		tri.a = 2;
		tri.b = 1;
		tri.c = 3;

		hullTris.push_back(tri);

		tri.a = 1;
		tri.b = 0;
		tri.c = 3;

		hullTris.push_back(tri);
	}

	void RemoveInternalPoints(const std::vector<glm::vec3>& hullPoints, std::vector<Triangle>& hullTris, std::vector<glm::vec3>& checkPts) {
		for (int i = 0; i < checkPts.size(); i++) {
			const glm::vec3& pt = checkPts[i];

			bool isExternal = false;
			for (int t = 0; t < hullTris.size(); t++) {
				const Triangle& tri = hullTris[t];
				const glm::vec3& a = hullPoints[tri.a];
				const glm::vec3& b = hullPoints[tri.b];
				const glm::vec3& c = hullPoints[tri.c];

				float dist = DistanceFromTriangle(a, b, c, pt);
				if (dist > 0.0f) { //the point is inside the triangle //changed here
					isExternal = true;
					break;
				}
			}

			if (!isExternal) {
				checkPts.erase(checkPts.begin() + i);
				i--;
			}
		}

		for (int i = 0; i < checkPts.size(); i++) {
			const glm::vec3& pt = checkPts[i];

			bool isTooClose = false;
			for (int j = 0; j < hullPoints.size(); j++) {
				glm::vec3 hullPt = hullPoints[j];
				glm::vec3 ray = hullPt - pt;
				if (glm::length2(ray) < 0.01f * 0.01f) { //changed here
					isTooClose = true;
					break;
				}
			}

			if (isTooClose) {
				checkPts.erase(checkPts.begin() + i);
				i--;
			}
		}
	}

	bool IsEdgeUnique(const std::vector<Triangle>& tris, const std::vector<int>& facingTris, const int ignoreTri, const Edge& edge) {
		for (int i = 0; i < facingTris.size(); i++) {
			const int triIdx = facingTris[i];
			if (ignoreTri == triIdx) {
				continue;
			}

			const Triangle& tri = tris[triIdx];

			Edge edges[3];
			edges[0] = Edge(tri.a, tri.b);

			edges[1] = Edge(tri.b, tri.c);

			edges[2] = Edge(tri.c, tri.a);

			for (int e = 0; e < 3; e++) {
				if (edge == edges[e]) {
					return false;
				}
			}
		}
		return true;
	}

	void AddPoint(std::vector<glm::vec3>& hullPoints, std::vector<Triangle>& hullTris, glm::vec3& pt) {
		//identify old triangles
		std::vector<int> facingTris;
		for (int i = (int)hullTris.size() - 1; i >= 0; i--) {
			const Triangle& tri = hullTris[i];


			const glm::vec3& a = hullPoints[tri.a];
			const glm::vec3& b = hullPoints[tri.b];
			const glm::vec3& c = hullPoints[tri.c];

			const float dist = DistanceFromTriangle(a, b, c, pt);
			if (dist > 0.0f) {
				facingTris.push_back(i);
			}
		}

		//add all the unique edges in the old triangles
		std::vector<Edge> uniqueEdges;
		for (int i = 0; i < facingTris.size(); i++) {
			const int triIdx = facingTris[i];
			const Triangle& tri = hullTris[triIdx];

			Edge edges[3];
			edges[0] = Edge(tri.a, tri.b);

			edges[1] = Edge(tri.b, tri.c);

			edges[2] = Edge(tri.c, tri.a);

			for (int e = 0; e < 3; e++) {
				if (IsEdgeUnique(hullTris, facingTris, triIdx, edges[e])) {
					uniqueEdges.push_back(edges[e]);
				}
			}
		}

		//remove old triangles
		for (int i = 0; i < facingTris.size(); i++) {
			hullTris.erase(hullTris.begin() + facingTris[i]);
		}

		//add new point
		hullPoints.push_back(pt);
		const int newPtIdx = (int)hullPoints.size() - 1;

		//add new triangles
		for (int i = 0; i < uniqueEdges.size(); i++) {
			const Edge& edge = uniqueEdges[i];

			Triangle tri(edge.a, edge.b, newPtIdx);

			hullTris.push_back(tri);
		}
	}

	void RemoveUnreferencedVerts(std::vector<glm::vec3>& hullPoints, std::vector<Triangle>& hullTris) {
		for (int i = 0; i < hullPoints.size(); i++) {
			bool isUsed = false;
			for (int j = 0; j < hullTris.size(); j++) {
				const Triangle& tri = hullTris[j];
				if (tri.a == i || tri.b == i || tri.c == i) {
					isUsed = true;
					break;
				}
			}

			if (isUsed) {
				continue;
			}

			for (int j = 0; j < hullTris.size(); j++) {
				Triangle& tri = hullTris[j];
				tri.a = (tri.a > i) ? tri.a - 1 : tri.a;//simplified the ifs, check here for error
				tri.b = (tri.b > i) ? tri.b - 1 : tri.b;
				tri.c = (tri.c > i) ? tri.c - 1 : tri.c;
			}

			hullPoints.erase(hullPoints.begin() + i);
			i--;
		}
	}

	void ExpandConverHull(std::vector<glm::vec3>& hullPoints, std::vector<Triangle>& hullTris, const std::vector<glm::vec3>& verts) {
		std::vector<glm::vec3> externalVerts = verts;
		RemoveInternalPoints(hullPoints, hullTris, externalVerts);


		while (externalVerts.size() > 0) {
			int ptIdx = FindPointFurthestInDir(externalVerts.data(), (int)externalVerts.size(), externalVerts[0]);
			glm::vec3 pt = externalVerts[ptIdx];

			externalVerts.erase(externalVerts.begin() + ptIdx);

			AddPoint(hullPoints, hullTris, pt);

			RemoveInternalPoints(hullPoints, hullTris, externalVerts);
		}

		RemoveUnreferencedVerts(hullPoints, hullTris);
	}

	void FurthestPoint::BuildConvexHull(std::vector<glm::vec3>& verts, std::vector<Triangle>& hullTris, bool removeUnused) {
		std::vector<glm::vec3>& hullPts = std::vector<glm::vec3>();
		BuildTetrahedron(verts.data(), (int)verts.size(), hullPts, hullTris);

		ExpandConverHull(hullPts, hullTris, verts);
		if (removeUnused)
		{
			verts = hullPts;
		}
	}
}