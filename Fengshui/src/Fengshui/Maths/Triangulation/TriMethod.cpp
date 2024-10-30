#include "fspch.h"
#include "TriMethod.h"
#include "Fengshui/PhysicsCustom/Maths/Bounds.h"

namespace Fengshui
{
	bool IsExternal(const std::vector< glm::vec3 >& pts, const std::vector<Triangle>& tris, const glm::vec3& pt) {
		bool isExternal = false;
		for (int t = 0; t < tris.size(); t++) {
			const Triangle& tri = tris[t];
			const glm::vec3& a = pts[tri.a];
			const glm::vec3& b = pts[tri.b];
			const glm::vec3& c = pts[tri.c];

			glm::vec3 ab = b - a;
			glm::vec3 ac = c - a;
			glm::vec3 normal = glm::normalize(glm::cross(ab, ac));

			glm::vec3 ray = pt - a;

			float dist = glm::dot(ray, normal);
			if (dist > 0.0f) {
				isExternal = true;
				break;
			}
		}
		return isExternal;
	}

	float TetrahedronVolume(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d) {
		const glm::vec3 ad = d - a;
		const glm::vec3 bd = d - b;
		const glm::vec3 cd = d - c;
		float volume = glm::dot(ad, glm::cross(bd, cd)) / 6.0f;

		return fabsf(volume);//return the absolute value
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

	glm::vec3 CalculateCenterOfMass(const std::vector<glm::vec3>& pts, const std::vector<Triangle>& tris) {
		const int numSamples = 100;//huh? it's sample rate right...

		Bounds bounds;
		bounds.Expand(&Vec3(pts.data()), (int)pts.size());

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

	glm::mat3 CalculateInertiaTensor(const std::vector<glm::vec3>& pts, const std::vector<Triangle>& tris, const glm::vec3& cm) {
		const int numSamples = 100;

		Bounds bounds;
		bounds.Expand(&Vec3(pts.data()), (int)pts.size());

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

	glm::vec3 TriMethod::CalculateCenterOfMassTetrahedron(const std::vector<glm::vec3>& pts, const std::vector<Triangle>& tris) {
		std::vector<glm::vec3> cms;
		float totalVolume = 0.0f;
		cms.reserve(tris.size());

		glm::vec3 centerish(0.0f);
		for (int i = 0; i < pts.size(); i++) {
			centerish += pts[i];
		}
		centerish *= 1.0f / float(pts.size());

		for (int i = 0; i < tris.size(); i++) {
			const Triangle& tri = tris[i];

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

	glm::mat3 TriMethod::CalculateInertiaTensorTetrahedron(const std::vector<glm::vec3>& pts, const std::vector<Triangle>& tris, const glm::vec3& cm) {
		glm::mat3 inertiaTensor = glm::mat3(0.0f);
		float totalVolume = 0.0f;

		for (int i = 0; i < tris.size(); i++) {
			const Triangle& tri = tris[i];

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
}