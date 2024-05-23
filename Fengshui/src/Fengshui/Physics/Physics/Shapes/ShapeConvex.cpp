//
//  ShapeConvex.cpp
//
#include "fspch.h"

#include "Fengshui/ECS/Components.h"

#include "ShapeConvex.h"
#include <glm/gtx/norm.hpp>
#include <glm/gtc/quaternion.hpp>

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

	void BuildTetrahedron(const glm::vec3* verts, const int num, std::vector<glm::vec3>& hullPts, std::vector<tri_t>& hullTris) {
		hullPts.clear();
		hullTris.clear();

		glm::vec3 points[4];

		int idx = FindPointFurthestInDir(verts, num, glm::vec3(1, 0, 0));
		points[0] = verts[idx];
		idx = FindPointFurthestInDir(verts, num, points[0] * -1.0f);
		points[1] = verts[idx];
		points[2] = FindPointFurthestFromLine(verts, num, points[0], points[1]);
		points[3] = FindPointFurthestFromTriangle(verts, num, points[0], points[1], points[2]);

		//need to make sure teh points are in counter clockwise order
		float dist = DistanceFromTriangle(points[0], points[1], points[2], points[3]);
		if (dist > 0.0f) {
			std::swap(points[0], points[1]);
		}

		hullPts.push_back(points[0]);
		hullPts.push_back(points[1]);
		hullPts.push_back(points[2]);
		hullPts.push_back(points[3]);

		tri_t tri;
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

	void RemoveInternalPoints(const std::vector<glm::vec3>& hullPoints, std::vector<tri_t>& hullTris, std::vector<glm::vec3>& checkPts) {
		for (int i = 0; i < checkPts.size(); i++) {
			const glm::vec3& pt = checkPts[i];

			bool isExternal = false;
			for (int t = 0; t < hullTris.size(); t++) {
				const tri_t& tri = hullTris[t];
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

	bool IsEdgeUnique(const std::vector<tri_t>& tris, const std::vector<int>& facingTris, const int ignoreTri, const edge_t& edge) {
		for (int i = 0; i < facingTris.size(); i++) {
			const int triIdx = facingTris[i];
			if (ignoreTri == triIdx) {
				continue;
			}

			const tri_t& tri = tris[triIdx];

			edge_t edges[3];
			edges[0].a = tri.a;
			edges[0].b = tri.b;

			edges[1].a = tri.b;
			edges[1].b = tri.c;

			edges[2].a = tri.c;
			edges[2].b = tri.a;

			for (int e = 0; e < 3; e++) {
				if (edge == edges[e]) {
					return false;
				}
			}
		}
		return true;
	}

	void AddPoint(std::vector<glm::vec3>& hullPoints, std::vector<tri_t>& hullTris, glm::vec3& pt) {
		//identify old triangles
		std::vector<int> facingTris;
		for (int i = (int)hullTris.size() - 1; i >= 0; i--) {
			const tri_t& tri = hullTris[i];


			const glm::vec3& a = hullPoints[tri.a];
			const glm::vec3& b = hullPoints[tri.b];
			const glm::vec3& c = hullPoints[tri.c];

			const float dist = DistanceFromTriangle(a, b, c, pt);
			if (dist > 0.0f) {
				facingTris.push_back(i);
			}
		}

		//add all the unique edges in the oldd triangles
		std::vector<edge_t> uniqueEdges;
		for (int i = 0; i < facingTris.size(); i++) {
			const int triIdx = facingTris[i];
			const tri_t& tri = hullTris[triIdx];

			edge_t edges[3];
			edges[0].a = tri.a;
			edges[0].b = tri.b;

			edges[1].a = tri.b;
			edges[1].b = tri.c;

			edges[2].a = tri.c;
			edges[2].b = tri.a;

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
			const edge_t& edge = uniqueEdges[i];

			tri_t tri;
			tri.a = edge.a;
			tri.b = edge.b;
			tri.c = newPtIdx;
			hullTris.push_back(tri);
		}
	}

	void RemoveUnreferencedVerts(std::vector<glm::vec3>& hullPoints, std::vector<tri_t>& hullTris) {
		for (int i = 0; i < hullPoints.size(); i++) {
			bool isUsed = false;
			for (int j = 0; j < hullTris.size(); j++) {
				const tri_t& tri = hullTris[j];
				if (tri.a == i || tri.b == i || tri.c == i) {
					isUsed = true;
					break;
				}
			}

			if (isUsed) {
				continue;
			}

			for (int j = 0; j < hullTris.size(); j++) {
				tri_t& tri = hullTris[j];
				tri.a = (tri.a > i) ? tri.a - 1 : tri.a;//simplified the ifs, check here for error
				tri.b = (tri.b > i) ? tri.b - 1 : tri.b;
				tri.c = (tri.c > i) ? tri.c - 1 : tri.c;
			}

			hullPoints.erase(hullPoints.begin() + i);
			i--;
		}
	}

	void ExpandConverHull(std::vector<glm::vec3>& hullPoints, std::vector<tri_t>& hullTris, const std::vector<glm::vec3>& verts) {
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

	bool IsExternal(const std::vector< glm::vec3 >& pts, const std::vector<tri_t>& tris, const glm::vec3& pt) {
		bool isExternal = false;
		for (int t = 0; t < tris.size(); t++) {
			const tri_t& tri = tris[t];
			const glm::vec3& a = pts[tri.a];
			const glm::vec3& b = pts[tri.b];
			const glm::vec3& c = pts[tri.c];

			float dist = DistanceFromTriangle(a, b, c, pt);
			if (dist > 0.0f) {
				isExternal = true;
				break;
			}
		}
		return isExternal;
	}

	/*
	* Extra content to speed up calculations
	*/
	float TetrahedronVolume(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d) {
		const glm::vec3 ad = d - a;
		const glm::vec3 bd = d - b;
		const glm::vec3 cd = d - c;
		float volume = glm::dot(ad, glm::cross(bd, cd)) / 6.0f;

		return fabsf(volume);//return the absolute value
	}

	glm::vec3 CalculateCenterOfMassTetrahedron(const std::vector<glm::vec3>& pts, const std::vector<tri_t>& tris) {
		std::vector<glm::vec3> cms;
		float totalVolume = 0.0f;
		cms.reserve(tris.size());

		glm::vec3 centerish(0.0f);
		for (int i = 0; i < pts.size(); i++) {
			centerish += pts[i];
		}
		centerish *= 1.0f / float(pts.size());

		for (int i = 0; i < tris.size(); i++) {
			const tri_t& tri = tris[i];

			const glm::vec3& ptA = centerish;
			const glm::vec3& ptB = pts[tri.a];
			const glm::vec3& ptC = pts[tri.b];
			const glm::vec3& ptD = pts[tri.c];

			const glm::vec3 centerOfMassOfSimplex = (ptA + ptB + ptC + ptD) * 0.25f;
			const float volume = TetrahedronVolume(ptA, ptB, ptC, ptD);
			cms.push_back(centerOfMassOfSimplex * volume);
			totalVolume += volume;
		}

		glm::vec3 cm(0.0f);
		for (int i = 0; i < cms.size(); i++) {
			cm += cms[i];
		}
		cm *= 1.0f / totalVolume;

		return cm;
	}

	glm::mat3 InertiaTensorTetrahedron(const glm::vec3& ptA, const glm::vec3& ptB, const glm::vec3& ptC, const glm::vec3& ptD) {
		const glm::vec3 pts[4] = { ptA, ptB, ptC, ptD };

		glm::mat3 mat;
		mat[0] = glm::vec3(pts[1].x - pts[0].x, pts[2].x - pts[0].x, pts[3].x - pts[0].x);
		mat[1] = glm::vec3(pts[1].y - pts[0].y, pts[2].y - pts[0].y, pts[3].y - pts[0].y);
		mat[2] = glm::vec3(pts[1].z - pts[0].z, pts[2].z - pts[0].z, pts[3].z - pts[0].z);
		const float DetJ = fabsf(glm::determinant(mat));

		const float density = 1.0f;
		const float mu = density;

		float xx = 0;
		float yy = 0;
		float zz = 0;

		float xy = 0;
		float xz = 0;
		float yz = 0;

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				//diagonals
				xx = pts[i].x * pts[j].x;
				yy = pts[i].y * pts[j].y;
				zz = pts[i].z * pts[j].z;

				//off-diagonals
				xy = pts[i].x * pts[j].y + pts[j].x * pts[i].y;
				xz = pts[i].x * pts[j].z + pts[j].x * pts[i].z;
				yz = pts[i].y * pts[j].z + pts[j].y * pts[i].z;
			}
		}

		const float a = mu * DetJ * (yy + zz) / 60.0f;
		const float b = mu * DetJ * (xx + zz) / 60.0f;
		const float c = mu * DetJ * (xx + yy) / 60.0f;

		const float aprime = mu * DetJ * yz / 120.0f;
		const float bprime = mu * DetJ * xz / 120.0f;
		const float cprime = mu * DetJ * xy / 120.0f;

		glm::mat3 inertiaTensor;
		inertiaTensor[0] = glm::vec3(a, -cprime, -bprime);
		inertiaTensor[1] = glm::vec3(-cprime, b, -aprime);
		inertiaTensor[2] = glm::vec3(-bprime, -aprime, c);
		return inertiaTensor;
	}

	glm::mat3 CalculateInertiaTensorTetrahedron(const std::vector<glm::vec3>& pts, const std::vector<tri_t>& tris, const glm::vec3& cm) {
		glm::mat3 inertiaTensor = glm::mat3(0.0f);
		float totalVolume = 0.0f;

		for (int i = 0; i < tris.size(); i++) {
			const tri_t& tri = tris[i];

			const glm::vec3 ptA = cm - cm;
			const glm::vec3 ptB = pts[tri.a] - cm;
			const glm::vec3 ptC = pts[tri.b] - cm;
			const glm::vec3 ptD = pts[tri.c] - cm;

			inertiaTensor += InertiaTensorTetrahedron(ptA, ptB, ptC, ptD);

			totalVolume += TetrahedronVolume(ptA, ptB, ptC, ptD);
		}

		inertiaTensor *= 1.0f / totalVolume;
		return inertiaTensor;
	}

	glm::vec3 CalculateCenterOfMass(const std::vector<glm::vec3>& pts, const std::vector<tri_t>& tris) {
		const int numSamples = 100;//huh? it's sample rate right...

		Bounds bounds;
		bounds.Expand(pts.data(), (int)pts.size());

		glm::vec3 cm(0.0f);
		const float dx = bounds.WidthX() / (float)numSamples;
		const float dy = bounds.WidthY() / (float)numSamples;
		const float dz = bounds.WidthZ() / (float)numSamples;

		int sampleCount = 0;
		for (float x = bounds.mins.x; x < bounds.maxs.x; x += dx) {
			for (float y = bounds.mins.y; y < bounds.maxs.y; y += dy) {
				for (float z = bounds.mins.z; z < bounds.maxs.z; z += dz) {
					glm::vec3 pt(x, y, z);

					if (IsExternal(pts, tris, pt)) {
						continue;
					}

					cm += pt;
					sampleCount++;
				}
			}
		}

		cm /= (float)sampleCount;
		return cm;
	}

	glm::mat3 CalculateInertiaTensor(const std::vector<glm::vec3>& pts, const std::vector<tri_t>& tris, const glm::vec3& cm) {
		const int numSamples = 100;

		Bounds bounds;
		bounds.Expand(pts.data(), (int)pts.size());

		glm::mat3 tensor = glm::mat3(0.0f);

		const float dx = bounds.WidthX() / (float)numSamples;
		const float dy = bounds.WidthY() / (float)numSamples;
		const float dz = bounds.WidthZ() / (float)numSamples;

		int sampleCount = 0;
		for (float x = bounds.mins.x; x < bounds.maxs.x; x += dx) {
			for (float y = bounds.mins.y; y < bounds.maxs.y; y += dy) {
				for (float z = bounds.mins.z; z < bounds.maxs.z; z += dz) {
					glm::vec3 pt(x, y, z);

					if (IsExternal(pts, tris, pt)) {
						continue;
					}

					pt -= cm;//displacement from center of mass

					tensor[0][0] += pt.y * pt.y + pt.z * pt.z;
					tensor[1][1] += pt.x * pt.x + pt.z * pt.z;
					tensor[2][2] += pt.x * pt.x + pt.y * pt.y;

					tensor[0][1] += -1.0f * pt.x * pt.y;
					tensor[0][2] += -1.0f * pt.x * pt.z;
					tensor[1][2] += -1.0f * pt.y * pt.z;

					tensor[1][0] += -1.0f * pt.x * pt.y;
					tensor[2][0] += -1.0f * pt.x * pt.z;
					tensor[2][1] += -1.0f * pt.y * pt.z;

					sampleCount++;
				}
			}
		}

		tensor *= 1.0f / (float)sampleCount;
		return tensor;
	}

	void BuildConvexHull(const std::vector< glm::vec3 >& verts, std::vector< glm::vec3 >& hullPts, std::vector< tri_t >& hullTris) {
		if (verts.size() < 4) {
			return;
		}

		BuildTetrahedron(verts.data(), (int)verts.size(), hullPts, hullTris);

		ExpandConverHull(hullPts, hullTris, verts);
	}

	/*
	========================================================================================================

	ShapeConvex

	========================================================================================================
	*/

	/*
	====================================================
	ShapeConvex::Build
	====================================================
	*/
	void ShapeConvex::Build(const glm::vec3* pts, const int num) {
		m_points.clear();
		m_points.reserve(num);
		for (int i = 0; i < num; i++) {
			m_points.push_back(pts[i]);
		}

		//Create the hull
		std::vector<glm::vec3> hullPoints;
		std::vector<tri_t> hullTriangles;
		BuildConvexHull(m_points, hullPoints, hullTriangles);
		m_points = hullPoints;

		//Expand the bounds
		m_bounds.Clear();
		m_bounds.Expand(m_points.data(), (int)m_points.size());
		m_centerOfMass = CalculateCenterOfMassTetrahedron(hullPoints, hullTriangles);
		m_inertiaTensor = CalculateInertiaTensorTetrahedron(hullPoints, hullTriangles, m_centerOfMass);
	}

	/*
	====================================================
	ShapeConvex::Support
	====================================================
	*/
	glm::vec3 ShapeConvex::Support(const glm::vec3& dir, const Transform transform, const float bias) const {
		glm::vec3 maxPt = (transform.Rotation * m_points[0]) * transform.Scale + transform.Position;
		float maxDist = glm::dot(dir, maxPt);
		for (int i = 1; i < m_points.size(); i++) {
			const glm::vec3 pt = (transform.Rotation * m_points[i]) * transform.Scale + transform.Position;
			const float dist = glm::dot(dir, pt);

			if (dist > maxDist) {
				maxDist = dist;
				maxPt = pt;
			}
		}

		glm::vec3 norm = glm::normalize(dir);
		norm *= bias;

		return maxPt + norm;
	}

	glm::mat3 ShapeConvex::InertiaTensor(const Transform transform) const
	{
		return m_inertiaTensor;
	}

	/*
	====================================================
	ShapeConvex::GetBounds
	====================================================
	*/
	Bounds ShapeConvex::GetBounds(const Transform transform) const {
		glm::vec3 corners[8];

		corners[0] = glm::vec3(m_bounds.mins.x, m_bounds.mins.y, m_bounds.mins.z);
		corners[1] = glm::vec3(m_bounds.mins.x, m_bounds.mins.y, m_bounds.maxs.z);
		corners[2] = glm::vec3(m_bounds.mins.x, m_bounds.maxs.y, m_bounds.mins.z);
		corners[3] = glm::vec3(m_bounds.maxs.x, m_bounds.mins.y, m_bounds.mins.z);

		corners[4] = glm::vec3(m_bounds.maxs.x, m_bounds.maxs.y, m_bounds.maxs.z);
		corners[5] = glm::vec3(m_bounds.maxs.x, m_bounds.maxs.y, m_bounds.mins.z);
		corners[6] = glm::vec3(m_bounds.maxs.x, m_bounds.mins.y, m_bounds.maxs.z);
		corners[7] = glm::vec3(m_bounds.mins.x, m_bounds.maxs.y, m_bounds.maxs.z);

		Bounds bounds;
		for (int i = 0; i < 8; i++) {
			bounds.Expand((transform.Rotation * corners[i]) * transform.Scale + transform.Position);
		}

		return bounds;
	}

	Bounds ShapeConvex::GetBounds(const glm::vec3 scale) const
	{
		glm::vec3 corners[8];

		corners[0] = glm::vec3(m_bounds.mins.x, m_bounds.mins.y, m_bounds.mins.z);
		corners[1] = glm::vec3(m_bounds.mins.x, m_bounds.mins.y, m_bounds.maxs.z);
		corners[2] = glm::vec3(m_bounds.mins.x, m_bounds.maxs.y, m_bounds.mins.z);
		corners[3] = glm::vec3(m_bounds.maxs.x, m_bounds.mins.y, m_bounds.mins.z);

		corners[4] = glm::vec3(m_bounds.maxs.x, m_bounds.maxs.y, m_bounds.maxs.z);
		corners[5] = glm::vec3(m_bounds.maxs.x, m_bounds.maxs.y, m_bounds.mins.z);
		corners[6] = glm::vec3(m_bounds.maxs.x, m_bounds.mins.y, m_bounds.maxs.z);
		corners[7] = glm::vec3(m_bounds.mins.x, m_bounds.maxs.y, m_bounds.maxs.z);

		Bounds bounds;
		for (int i = 0; i < 8; i++) {
			bounds.Expand(corners[i] * scale);
		}

		return bounds;
	}

	/*
	====================================================
	ShapeConvex::FastestLinearSpeed
	====================================================
	*/
	float ShapeConvex::FastestLinearSpeed(const glm::vec3& angularVelocity, const glm::vec3& dir) const {
		float maxSpeed = 0.0f;
		for (int i = 0; i < m_points.size(); i++) {
			glm::vec3 r = m_points[i] - m_centerOfMass;
			glm::vec3 linearVelocity = glm::cross(angularVelocity, r);//perpendicular component of the angular speed
			float speed = glm::dot(dir, linearVelocity);//component of the perpendicular speed in direction of dir
			if (speed > maxSpeed) {
				maxSpeed = speed;
			}
		}

		return maxSpeed;
	}
}