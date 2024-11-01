//
//  GJK.cpp
//
#include "fspch.h"

#include "GJK.h"

namespace Fengshui
{
	struct point_t;
	float EPA_Expand(const Collider& colliderA, const Transform& transA, const Collider& colliderB, const Transform& transB, const float bias, const point_t simplexPoints[4], glm::vec3& ptOnA, glm::vec3& ptOnB);


	glm::vec2 SignedVolume1D(const glm::vec3& s1, const glm::vec3& s2) {
		glm::vec3 ab = s2 - s1;
		glm::vec3 ap = glm::vec3(0.0f) - s1;
		glm::vec3 p0 = s1 + ab * glm::dot(ab, ap) / glm::length2(ab); // project origin onto the line. Huh?

		int idx = 0;
		float mu_max = 0;
		for (int i = 0; i < 3; i++) {
			float mu = s2[i] - s1[i];
			if (mu * mu > mu_max * mu_max) {
				mu_max = mu;
				idx = i;
			}
		}

		const float a = s1[idx];
		const float b = s2[idx];
		const float p = p0[idx];

		const float C1 = p - a;
		const float C2 = b - p;

		if ((p > a && p < b) || (p<a && p>b)) {
			glm::vec2 lambdas;
			lambdas[0] = C2 / mu_max;
			lambdas[1] = C1 / mu_max;
			return lambdas;
		}

		if ((a <= b && p <= a) || (a >= b && p >= a)) {
			return glm::vec2(1.0f, 0.0f);
		}

		return glm::vec2(0.0f, 1.0f);
	}

	int CompareSigns(float a, float b) {
		if (a > 0.0f && b > 0.0f) {
			return 1;
		}

		if (a < 0.0f && b < 0.0f) {
			return 1;
		}

		return 0;
	}

	glm::vec3 SignedVolume2D(const glm::vec3& s1, const glm::vec3& s2, const glm::vec3& s3) {
		glm::vec3 normal = glm::cross((s2 - s1),(s3 - s1));
		glm::vec3 p0 = normal * glm::dot(s1,normal) / glm::length2(normal);

		int idx = 0;
		float area_max = 0;

		for (int i = 0; i < 3; i++) {
			int j = (i + 1) % 3;
			int k = (i + 2) % 3;

			glm::vec2 a = glm::vec2(s1[j], s1[k]);
			glm::vec2 b = glm::vec2(s2[j], s2[k]);
			glm::vec2 c = glm::vec2(s3[j], s3[k]);
			glm::vec2 ab = b - a;
			glm::vec2 ac = c - a;

			float area = ab.x * ac.y - ab.y * ac.x;
			if (area * area > area_max * area_max) {
				idx = i;
				area_max = area;
			}
		}

		int x = (idx + 1) % 3;
		int y = (idx + 2) % 3;
		glm::vec2 s[3];
		s[0] = glm::vec2(s1[x], s1[y]);
		s[1] = glm::vec2(s2[x], s2[y]);
		s[2] = glm::vec2(s3[x], s3[y]);
		glm::vec2 p = glm::vec2(p0[x], p0[y]);

		glm::vec3 areas;
		for (int i = 0; i < 3; i++) {
			int j = (i + 1) % 3;
			int k = (i + 2) % 3;

			glm::vec2 a = p;
			glm::vec2 b = s[j];
			glm::vec2 c = s[k];
			glm::vec2 ab = b - a;
			glm::vec2 ac = c - a;

			areas[i] = ab.x * ac.y - ab.y * ac.x;
		}

		if (CompareSigns(area_max, areas[0]) > 0 && CompareSigns(area_max, areas[1]) > 0 && CompareSigns(area_max, areas[2]) > 0) {
			return areas / area_max;
		}

		float dist = 1e10;//manual maximum
		glm::vec3 lambdas = glm::vec3(1, 0, 0);
		for (int i = 0; i < 3; i++) {
			int j = (i + 1) % 3;
			int k = (i + 2) % 3;

			glm::vec3 edgesPts[3];
			edgesPts[0] = s1;
			edgesPts[1] = s2;
			edgesPts[2] = s3;

			glm::vec2 lambdaEdge = SignedVolume1D(edgesPts[j], edgesPts[k]);
			glm::vec3 pt = edgesPts[j] * lambdaEdge[0] + edgesPts[k] * lambdaEdge[1];
			if (glm::length2(pt) < dist) {
				dist = glm::length2(pt);
				lambdas[i] = 0;
				lambdas[j] = lambdaEdge[0];
				lambdas[k] = lambdaEdge[1];
			}
		}

		return lambdas;
	}

	glm::vec4 SignedVolume3D(const glm::vec3& s1, const glm::vec3& s2, const glm::vec3& s3, const glm::vec3& s4) {
		glm::mat4 M;
		M[0] = glm::vec4(s1.x, s2.x, s3.x, s4.x);
		M[1] = glm::vec4(s1.y, s2.y, s3.y, s4.y);
		M[2] = glm::vec4(s1.z, s2.z, s3.z, s4.z);
		M[3] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		M = glm::transpose(M);

		glm::vec4 C4;
		C4[0] = Cofactor(M, 3, 0);
		C4[1] = Cofactor(M, 3, 1);
		C4[2] = Cofactor(M, 3, 2);
		C4[3] = Cofactor(M, 3, 3);

		const float detM = C4[0] + C4[1] + C4[2] + C4[3];

		if (CompareSigns(detM, C4[0]) > 0 && CompareSigns(detM, C4[1]) > 0
			&& CompareSigns(detM, C4[2]) > 0 && CompareSigns(detM, C4[3]) > 0) {
			return C4 * (1.0f / detM);
		}

		glm::vec4 lambdas;
		float dist = 1e10;//manual maximum
		for (int i = 0; i < 4; i++) {
			int j = (i + 1) % 4;
			int k = (i + 2) % 4;

			glm::vec3 facePts[4];
			facePts[0] = s1;
			facePts[1] = s2;
			facePts[2] = s3;
			facePts[3] = s4;

			glm::vec3 lambdaFace = SignedVolume2D(facePts[i], facePts[j], facePts[k]);
			glm::vec3 pt = facePts[i] * lambdaFace[0] + facePts[j] * lambdaFace[1] + facePts[k] * lambdaFace[2];
			if (glm::length2(pt) < dist) {
				dist = glm::length2(pt);
				lambdas = glm::vec4(0.0f);
				lambdas[i] = lambdaFace[0];
				lambdas[j] = lambdaFace[1];
				lambdas[k] = lambdaFace[2];
			}
		}

		return lambdas;
	}

	struct point_t {
		glm::vec3 xyz; //point on minkowski sum
		glm::vec3 ptA; //point on bodyA
		glm::vec3 ptB; //point on bodyB

		point_t() : xyz(0.0f), ptA(0.0f), ptB(0.0f) {}

		const point_t& operator= (const point_t& rhs) {
			xyz = rhs.xyz;
			ptA = rhs.ptA;
			ptB = rhs.ptB;
			return *this;
		}

		bool operator== (const point_t& rhs) const {
			return (ptA == rhs.ptA) && (ptB == rhs.ptB) && (xyz == rhs.xyz);
		}
	};

	point_t Support(const Collider& colliderA, const Transform& transA, const Collider& colliderB, const Transform& transB, glm::vec3 inDir, const float bias) {
		glm::vec3 dir = glm::normalize(inDir);

		point_t point;

		point.ptA = colliderA.Shape->Support(dir, transA, bias);

		dir *= -1.0f;

		point.ptB = colliderB.Shape->Support(dir, transB, bias);

		point.xyz = point.ptA - point.ptB;//A-B huh
		return point;
	}

	bool SimplexSignedVolumes(point_t* pts, const int num, glm::vec3& newDir, glm::vec4& lambdasOut) {
		const float epsilonf = 0.0001f * 0.0001f;
		lambdasOut = glm::vec4(0.0f);

		bool doesIntersect = false;
		switch (num) {
		default:
		case 2: {
			glm::vec2 lambdas = SignedVolume1D(pts[0].xyz, pts[1].xyz);
			glm::vec3 v(0.0f);
			for (int i = 0; i < 2; i++) {
				v += pts[i].xyz * lambdas[i];
			}
			newDir = v * -1.0f;
			doesIntersect = (glm::length2(v) < epsilonf);
			lambdasOut[0] = lambdas[0];
			lambdasOut[1] = lambdas[1];
		} break;
		case 3: {
			glm::vec3 lambdas = SignedVolume2D(pts[0].xyz, pts[1].xyz, pts[2].xyz);
			glm::vec3 v(0.0f);
			for (int i = 0; i < 3; i++) {
				v += pts[i].xyz * lambdas[i];
			}
			newDir = v * -1.0f;
			doesIntersect = (glm::length2(v) < epsilonf);
			lambdasOut[0] = lambdas[0];
			lambdasOut[1] = lambdas[1];
			lambdasOut[2] = lambdas[2];
		} break;
		case 4: {
			glm::vec4 lambdas = SignedVolume3D(pts[0].xyz, pts[1].xyz, pts[2].xyz, pts[3].xyz);
			glm::vec3 v(0.0f);
			for (int i = 0; i < 4; i++) {
				v += pts[i].xyz * lambdas[i];
			}
			newDir = v * -1.0f;
			doesIntersect = (glm::length2(v) < epsilonf);
			lambdasOut[0] = lambdas[0];
			lambdasOut[1] = lambdas[1];
			lambdasOut[2] = lambdas[2];
			lambdasOut[3] = lambdas[3];
		} break;
		}

		return doesIntersect;
	}

	bool HasPoint(const point_t simplexPoints[4], const point_t& newPt) {
		const float precision = 1e-6f;//a good precision for approximation

		for (int i = 0; i < 4; i++) {
			glm::vec3 delta = simplexPoints[i].xyz - newPt.xyz;
			if (glm::length2(delta) < precision * precision) {
				return true;
			}
		}
		return false;
	}

	void SortValids(point_t simplexPoints[4], glm::vec4& lambdas) {
		bool valids[4];
		//record the valid indexes
		for (int i = 0; i < 4; i++) {
			valids[i] = (lambdas[i] != 0);
		}

		glm::vec4 validLambdas(0.0f);
		int validCount = 0;
		point_t validPts[4];
		memset(validPts, 0, sizeof(point_t) * 4);//set the array to 0, is that the best way to do it?
		for (int i = 0; i < 4; i++) {
			if (valids[i]) {
				validPts[validCount] = simplexPoints[i];
				validLambdas[validCount] = lambdas[i];
				validCount++;
			}
		}

		for (int i = 0; i < 4; i++) {
			simplexPoints[i] = validPts[i];
			lambdas[i] = validLambdas[i];
		}
	}

	static int NumValids(const glm::vec4& lambdas) {
		int num = 0;
		for (int i = 0; i < 4; i++) {
			if (lambdas[i] != 0.0f) {
				num++;
			}
		}
		return num;
	}

	/*
	================================
	GJK_DoesIntersect
	================================
	*/
	bool GJK_DoesIntersect(const Collider& colliderA, const Transform& transA, const Collider& colliderB, const Transform& transB) {
		const glm::vec3 origin(0.0f);

		int numPts = 1;
		point_t simplexPoints[4];
		simplexPoints[0] = Support(colliderA, transA, colliderB, transB, glm::vec3(1, 1, 1), 0.0f);

		float closestDist = 1e10f;//manual unlikely upperbound, like python's math.inf
		bool doesContainOrigin = false;
		glm::vec3 newDir = simplexPoints[0].xyz * -1.0f;

		//iteratively fill the tetrahedron
		do {
			point_t newPt = Support(colliderA, transA, colliderB, transB, newDir, 0.0f);

			//if the new point has been found before, we cannot expand further
			if (HasPoint(simplexPoints, newPt)) {
				break;
			}

			//add the new point to simplex points
			simplexPoints[numPts] = newPt;
			numPts++;

			//if the new point(furthest point) is not on the other side of the origin, then we will never include origin so break
			if (glm::dot(newDir, (newPt.xyz - origin)) < 0.0f) {
				break;
			}

			glm::vec4 lambdas;
			doesContainOrigin = SimplexSignedVolumes(simplexPoints, numPts, newDir, lambdas);
			if (doesContainOrigin) {
				break;
			}

			//The new projection is further from the origin point hence we cannot improve further
			float dist = glm::length2(newDir);
			if (dist >= closestDist) {
				break;
			}
			closestDist = dist;

			SortValids(simplexPoints, lambdas);
			numPts = NumValids(lambdas);
			doesContainOrigin = (numPts == 4);
		} while (!doesContainOrigin);

		return doesContainOrigin;
	}

	/*
	================================
	GJK_ClosestPoints
	================================
	*/
	void GJK_ClosestPoints(const Collider& colliderA, const Transform& transA, const Collider& colliderB, const Transform& transB, glm::vec3& ptOnA, glm::vec3& ptOnB) {
		const glm::vec3 origin(0.0f);

		float closestDist = 1e10f;
		const float bias = 0.0f;

		int numPts = 1;
		point_t simplexPoints[4];
		simplexPoints[0] = Support(colliderA, transA, colliderB, transB, glm::vec3(1, 1, 1), bias);

		glm::vec4 lambdas = glm::vec4(1, 0, 0, 0);
		glm::vec3 newDir = simplexPoints[0].xyz * -1.0f;

		do {
			point_t newPt = Support(colliderA, transA, colliderB, transB, newDir, bias);

			if (HasPoint(simplexPoints, newPt)) {
				break;
			}

			simplexPoints[numPts] = newPt;
			numPts++;

			SimplexSignedVolumes(simplexPoints, numPts, newDir, lambdas);
			SortValids(simplexPoints, lambdas);
			numPts = NumValids(lambdas);

			float dist = glm::length2(newDir);
			if (dist >= closestDist) {
				break;
			}
			closestDist = dist;
		} while (numPts < 4);

		//calculate the points on A and B with projection
		ptOnA = glm::vec3(0.0f);
		ptOnB = glm::vec3(0.0f);
		for (int i = 0; i < 4; i++) {
			ptOnA += simplexPoints[i].ptA * lambdas[i];
			ptOnB += simplexPoints[i].ptB * lambdas[i];
		}
	}

	/*
	================================
	GJK_DoesIntersect
	================================
	*/
	bool GJK_DoesIntersect(const Collider& colliderA, const Transform& transA, const Collider& colliderB, const Transform& transB, const float bias, glm::vec3& ptOnA, glm::vec3& ptOnB) {
		const glm::vec3 origin(0.0f);

		int numPts = 1;
		point_t simplexPoints[4];
		simplexPoints[0] = Support(colliderA, transA, colliderB, transB, glm::vec3(1, 1, 1), 0.0f);

		float closestDist = 1e10f;//manual unlikely upperbound, like python's math.inf
		bool doesContainOrigin = false;
		glm::vec3 newDir = simplexPoints[0].xyz * -1.0f;

		//iteratively fill the tetrahedron
		do {
			point_t newPt = Support(colliderA, transA, colliderB, transB, newDir, 0.0f);

			//if the new point has been found before, we cannot expand further
			if (HasPoint(simplexPoints, newPt)) {
				break;
			}

			//add the new point to simplex points
			simplexPoints[numPts] = newPt;
			numPts++;

			//if the new point(furthest point) is not on the other side of the origin, then we will never include origin so break
			if (glm::dot(newDir,(newPt.xyz - origin)) < 0.0f) {
				break;
			}

			glm::vec4 lambdas;
			doesContainOrigin = SimplexSignedVolumes(simplexPoints, numPts, newDir, lambdas);
			if (doesContainOrigin) {
				break;
			}

			//The new projection is further from the origin point hence we cannot improve further
			float dist = glm::length2(newDir);
			if (dist >= closestDist) {
				break;
			}
			closestDist = dist;

			SortValids(simplexPoints, lambdas);
			numPts = NumValids(lambdas);
			doesContainOrigin = (numPts == 4);
		} while (!doesContainOrigin);

		if (!doesContainOrigin) {
			return false;
		}

		//makes sure we have a tetrahedron for EPA
		if (numPts == 1) {
			glm::vec3 searchDir = simplexPoints[0].xyz * -1.0f;
			simplexPoints[numPts] = Support(colliderA, transA, colliderB, transB, searchDir, 0.0f);
			numPts++;
		}
		if (numPts == 2) {
			glm::vec3 ab = simplexPoints[1].xyz - simplexPoints[0].xyz;
			glm::vec3 u, v;
			GetOrtho(ab, u, v);
			simplexPoints[numPts] = Support(colliderA, transA, colliderB, transB, u, 0.0f);
			numPts++;
		}
		if (numPts == 3) {
			glm::vec3 ab = simplexPoints[1].xyz - simplexPoints[0].xyz;
			glm::vec3 ac = simplexPoints[2].xyz - simplexPoints[0].xyz;
			glm::vec3 norm = glm::cross(ab, ac);
			simplexPoints[numPts] = Support(colliderA, transA, colliderB, transB, norm, 0.0f);
			numPts++;
		}

		//get the center of mass for simplex
		glm::vec3 avg = glm::vec3(0, 0, 0);
		for (int i = 0; i < 4; i++) {
			avg += simplexPoints[i].xyz;
		}
		avg *= 0.25f;

		//add bias
		for (int i = 0; i < numPts; i++) {
			point_t& pt = simplexPoints[i];

			glm::vec3 dir = glm::normalize(pt.xyz - avg);
			pt.ptA += dir * bias;
			pt.ptB -= dir * bias;
			pt.xyz = pt.ptA - pt.ptB;
		}

		EPA_Expand(colliderA, transA, colliderB, transB, bias, simplexPoints, ptOnA, ptOnB);
		return true;
	}

	//find the center of mass for the points
	glm::vec3 BarycentricCoordinates(glm::vec3 s1, glm::vec3 s2, glm::vec3 s3, const glm::vec3& pt) {
		s1 = s1 - pt;
		s2 = s2 - pt;
		s3 = s3 - pt;

		glm::vec3 normal = glm::cross((s2 - s1), (s3 - s1));
		glm::vec3 p0 = normal * glm::dot(s1, normal) / glm::length2(normal);

		int idx = 0;
		float area_max = 0;
		for (int i = 0; i < 3; i++) {
			int j = (i + 1) % 3;
			int k = (i + 2) % 3;

			glm::vec2 a = glm::vec2(s1[j], s1[k]);
			glm::vec2 b = glm::vec2(s2[j], s2[k]);
			glm::vec2 c = glm::vec2(s3[j], s3[k]);
			glm::vec2 ab = b - a;
			glm::vec2 ac = c - a;

			float area = ab.x * ac.y - ab.y * ac.x;
			if (area * area > area_max * area_max) {
				idx = i;
				area_max = area;
			}
		}

		int x = (idx + 1) % 3;
		int y = (idx + 2) % 3;

		glm::vec2 s[3];
		s[0] = glm::vec2(s1[x], s1[y]);
		s[1] = glm::vec2(s2[x], s2[y]);
		s[2] = glm::vec2(s3[x], s3[y]);
		glm::vec2 p = glm::vec2(p0[x], p0[y]);

		glm::vec3 areas;
		for (int i = 0; i < 3; i++) {
			int j = (i + 1) % 3;
			int k = (i + 2) % 3;

			glm::vec2 a = p;
			glm::vec2 b = s[j];
			glm::vec2 c = s[k];
			glm::vec2 ab = b - a;
			glm::vec2 ac = c - a;

			areas[i] = ab.x * ac.y - ab.y * ac.x;
		}

		glm::vec3 lambdas = areas / area_max;
		if (!IsValid(lambdas)) {
			lambdas = glm::vec3(1, 0, 0);//a bit reckless here oof, but works I guess
		}
		return lambdas;
	}

	glm::vec3 NormalDirection(const Triangle& tri, const std::vector<point_t>& points) {
		const glm::vec3& a = points[tri.a].xyz;
		const glm::vec3& b = points[tri.b].xyz;
		const glm::vec3& c = points[tri.c].xyz;

		glm::vec3 ab = b - a;
		glm::vec3 ac = c - a;
		glm::vec3 normal = glm::normalize(glm::cross(ab, ac));
		return normal;
	}

	float SignedDistanceToTriangle(const Triangle& tri, const glm::vec3& pt, const std::vector<point_t>& points) {
		const glm::vec3 normal = NormalDirection(tri, points);
		const glm::vec3& a = points[tri.a].xyz;
		const float dist = glm::dot(normal, (pt - a));
		return dist;
	}

	int ClosestTriangle(const std::vector<Triangle>& triangles, const std::vector<point_t>& points) {
		float minDistSqr = 1e10;//manual unlikely upperbound, like python's math.inf

		int idx = -1;
		for (int i = 0; i < triangles.size(); i++) {
			const Triangle& tri = triangles[i];

			float dist = SignedDistanceToTriangle(tri, glm::vec3(0.0f), points);
			float distSqr = dist * dist;
			if (distSqr < minDistSqr) {
				idx = i;
				minDistSqr = distSqr;
			}
		}

		return idx;
	}

	bool HasPoint(const glm::vec3& w, const std::vector<Triangle> triangles, const std::vector<point_t>& points) {
		const float epsilons = 0.001f * 0.001f;
		glm::vec3 delta;

		for (int i = 0; i < triangles.size(); i++) {
			const Triangle& tri = triangles[i];

			delta = w - points[tri.a].xyz;
			if (glm::length2(delta) < epsilons) {
				return true;
			}
			delta = w - points[tri.b].xyz;
			if (glm::length2(delta) < epsilons) {
				return true;
			}
			delta = w - points[tri.c].xyz;
			if (glm::length2(delta) < epsilons) {
				return true;
			}
		}
		return false;
	}

	int RemoveTrianglesFacingPoint(const glm::vec3& pt, std::vector<Triangle>& triangles, const std::vector<point_t>& points) {
		int numRemoved = 0;
		for (int i = 0; i < triangles.size(); i++) {
			const Triangle& tri = triangles[i];

			float dist = SignedDistanceToTriangle(tri, pt, points);
			if (dist > 0.0f) {//this triangle faces the point
				triangles.erase(triangles.begin() + i);
				i--;
				numRemoved++;
			}
		}
		return numRemoved;
	}

	void FindDanglingEdges(std::vector<Edge>& danglingEdges, const std::vector<Triangle>& triangles) {
		danglingEdges.clear();

		for (int i = 0; i < triangles.size(); i++) {
			const Triangle tri = triangles[i];

			Edge edges[3];
			edges[0].a = tri.a;
			edges[0].b = tri.b;

			edges[1].a = tri.b;
			edges[1].b = tri.c;

			edges[2].a = tri.c;
			edges[2].b = tri.a;

			int counts[3];
			counts[0] = 0;
			counts[1] = 0;
			counts[2] = 0;

			for (int j = 0; j < triangles.size(); j++) {
				if (j == i) {
					continue;
				}

				const Triangle tri2 = triangles[j];

				Edge edges2[3];
				edges2[0].a = tri2.a;
				edges2[0].b = tri2.b;

				edges2[1].a = tri2.b;
				edges2[1].b = tri2.c;

				edges2[2].a = tri2.c;
				edges2[2].b = tri2.a;

				for (int k = 0; k < 3; k++) {
					if (edges[k] == edges2[0]) {
						counts[k]++;
					}
					if (edges[k] == edges2[1]) {
						counts[k]++;
					}
					if (edges[k] == edges2[2]) {
						counts[k]++;
					}
				}
			}

			for (int k = 0; k < 3; k++) {
				if (counts[k] == 0) {
					danglingEdges.push_back(edges[k]);
				}
			}
		}
	}

	float EPA_Expand(const Collider& colliderA, const Transform& transA, const Collider& colliderB, const Transform& transB, const float bias, const point_t simplexPoints[4], glm::vec3& ptOnA, glm::vec3& ptOnB) {
		std::vector<point_t> points;
		std::vector<Triangle> triangles;
		std::vector<Edge> danglingEdges;

		glm::vec3 center(0.0f);
		for (int i = 0; i < 4; i++) {
			points.push_back(simplexPoints[i]);
			center += simplexPoints[i].xyz;
		}
		center *= 0.25f;

		for (int i = 0; i < 4; i++) {
			int j = (i + 1) % 4;
			int k = (i + 2) % 4;

			Triangle tri;
			tri.a = i;
			tri.b = j;
			tri.c = k;

			int unusedPt = (i + 3) % 4;
			float dist = SignedDistanceToTriangle(tri, points[unusedPt].xyz, points);

			if (dist > 0.0f) {
				std::swap(tri.a, tri.b);
			}

			triangles.push_back(tri);
		}

		while (1) {
			const int idx = ClosestTriangle(triangles, points);
			glm::vec3 normal = NormalDirection(triangles[idx], points);

			const point_t newPt = Support(colliderA, transA, colliderB, transB, normal, bias);

			//point is seen, terminate the loop
			if (HasPoint(newPt.xyz, triangles, points)) {
				break;
			}

			//check if it is on the outside of the triangle, if not terminate
			float dist = SignedDistanceToTriangle(triangles[idx], newPt.xyz, points);
			if (dist <= 0.0f) {
				break;
			}

			const int newIdx = (int)points.size();
			points.push_back(newPt);

			//remove triangles facing the new point, if there is none, we have done expanding
			if (RemoveTrianglesFacingPoint(newPt.xyz, triangles, points) == 0) {
				break;
			}

			//find all the dangling edges, if there is none, we have reached maximum expansion and terminate
			//FROM BOOK: the points should theoretically be CCW, so adding new point as a should make the triangle face away from origin
			danglingEdges.clear();
			FindDanglingEdges(danglingEdges, triangles);
			if (danglingEdges.size() == 0) {
				break;
			}

			//form new triangles using the newly added point and the dangling edges
			for (int i = 0; i < danglingEdges.size(); i++) {
				const Edge& edge = danglingEdges[i];

				Triangle triangle;
				triangle.a = newIdx;
				triangle.b = edge.b;
				triangle.c = edge.a;

				//make sure it is facing out, ie the distance to center is negative
				float dist = SignedDistanceToTriangle(triangle, center, points);
				if (dist > 0.0f) {
					std::swap(triangle.b, triangle.c);
				}

				triangles.push_back(triangle);
			}
		}

		//get projection of the origin on the closest triangle
		const int idx = ClosestTriangle(triangles, points);
		const Triangle& tri = triangles[idx];
		glm::vec3 ptA_w = points[tri.a].xyz;
		glm::vec3 ptB_w = points[tri.b].xyz;
		glm::vec3 ptC_w = points[tri.c].xyz;
		//bias is necessary so point A and point B are not too close as that could cause issue calculating contact normal
		glm::vec3 lambdas = BarycentricCoordinates(ptA_w, ptB_w, ptC_w, glm::vec3(0.0f));//last term is a bias subject to change

		//get the point on body A
		glm::vec3 ptA_a = points[tri.a].ptA;
		glm::vec3 ptB_a = points[tri.b].ptA;
		glm::vec3 ptC_a = points[tri.c].ptA;
		ptOnA = ptA_a * lambdas[0] + ptB_a * lambdas[1] + ptC_a * lambdas[2];

		//get the point on body B
		glm::vec3 ptA_b = points[tri.a].ptB;
		glm::vec3 ptB_b = points[tri.b].ptB;
		glm::vec3 ptC_b = points[tri.c].ptB;
		ptOnB = ptA_b * lambdas[0] + ptB_b * lambdas[1] + ptC_b * lambdas[2];

		//return penetration distance
		glm::vec3 delta = ptOnB - ptOnA;
		return glm::length(delta);
	}
}