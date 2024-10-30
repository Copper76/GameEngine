//
//  Shapes.cpp
//
#include "fspch.h"

#include "Fengshui/ECS/Components.h"

#include "PhysicalShapeBox.h"
#include <glm/gtx/norm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Fengshui
{
	void PhysicalShapeBox::Build(const glm::vec3* pts, const int num) {
		for (int i = 0; i < num; i++) {
			m_bounds.Expand(Vec3(pts[i]));
			m_BaseCoords[i] = pts[i];
		}

		m_VertexCoords.clear();
		m_VertexCoords.push_back(glm::vec3(m_bounds.mins.x, m_bounds.mins.y, m_bounds.mins.z));
		m_VertexCoords.push_back(glm::vec3(m_bounds.maxs.x, m_bounds.mins.y, m_bounds.mins.z));
		m_VertexCoords.push_back(glm::vec3(m_bounds.mins.x, m_bounds.maxs.y, m_bounds.mins.z));
		m_VertexCoords.push_back(glm::vec3(m_bounds.mins.x, m_bounds.mins.y, m_bounds.maxs.z));

		m_VertexCoords.push_back(glm::vec3(m_bounds.maxs.x, m_bounds.maxs.y, m_bounds.maxs.z));
		m_VertexCoords.push_back(glm::vec3(m_bounds.mins.x, m_bounds.maxs.y, m_bounds.maxs.z));
		m_VertexCoords.push_back(glm::vec3(m_bounds.maxs.x, m_bounds.mins.y, m_bounds.maxs.z));
		m_VertexCoords.push_back(glm::vec3(m_bounds.maxs.x, m_bounds.maxs.y, m_bounds.mins.z));

		m_CenterOfMass = (m_bounds.maxs + m_bounds.mins).ToGLM() * 0.5f;
	}

	void PhysicalShapeBox::Build(const glm::vec3* pts, const int num, const glm::vec3 offset, const glm::vec3 scale)
	{
		for (int i = 0; i < num; i++) {
			m_bounds.Expand((pts[i] * scale) + offset);
			m_BaseCoords[i] = pts[i];
		}

		m_VertexCoords.clear();
		m_VertexCoords.push_back(glm::vec3(m_bounds.mins.x, m_bounds.mins.y, m_bounds.mins.z));
		m_VertexCoords.push_back(glm::vec3(m_bounds.maxs.x, m_bounds.mins.y, m_bounds.mins.z));
		m_VertexCoords.push_back(glm::vec3(m_bounds.mins.x, m_bounds.maxs.y, m_bounds.mins.z));
		m_VertexCoords.push_back(glm::vec3(m_bounds.mins.x, m_bounds.mins.y, m_bounds.maxs.z));

		m_VertexCoords.push_back(glm::vec3(m_bounds.maxs.x, m_bounds.maxs.y, m_bounds.maxs.z));
		m_VertexCoords.push_back(glm::vec3(m_bounds.mins.x, m_bounds.maxs.y, m_bounds.maxs.z));
		m_VertexCoords.push_back(glm::vec3(m_bounds.maxs.x, m_bounds.mins.y, m_bounds.maxs.z));
		m_VertexCoords.push_back(glm::vec3(m_bounds.maxs.x, m_bounds.maxs.y, m_bounds.mins.z));

		m_CenterOfMass = (m_bounds.maxs + m_bounds.mins).ToGLM() * 0.5f;
	}

	void PhysicalShapeBox::Rebuild(const glm::vec3 offset, const glm::vec3 scale)
	{
		for (glm::vec3 pt : m_BaseCoords) {
			m_bounds.Expand((pt * scale) + offset);
		}

		m_VertexCoords.clear();
		m_VertexCoords.push_back(glm::vec3(m_bounds.mins.x, m_bounds.mins.y, m_bounds.mins.z));
		m_VertexCoords.push_back(glm::vec3(m_bounds.maxs.x, m_bounds.mins.y, m_bounds.mins.z));
		m_VertexCoords.push_back(glm::vec3(m_bounds.mins.x, m_bounds.maxs.y, m_bounds.mins.z));
		m_VertexCoords.push_back(glm::vec3(m_bounds.mins.x, m_bounds.mins.y, m_bounds.maxs.z));

		m_VertexCoords.push_back(glm::vec3(m_bounds.maxs.x, m_bounds.maxs.y, m_bounds.maxs.z));
		m_VertexCoords.push_back(glm::vec3(m_bounds.mins.x, m_bounds.maxs.y, m_bounds.maxs.z));
		m_VertexCoords.push_back(glm::vec3(m_bounds.maxs.x, m_bounds.mins.y, m_bounds.maxs.z));
		m_VertexCoords.push_back(glm::vec3(m_bounds.maxs.x, m_bounds.maxs.y, m_bounds.mins.z));

		m_CenterOfMass = (m_bounds.maxs + m_bounds.mins).ToGLM() * 0.5f;
	}

	/*
	====================================================
	ShapeBox::Support
	====================================================
	*/
	glm::vec3 PhysicalShapeBox::Support(const glm::vec3& dir, const Transform& transform, const float bias) const {
		glm::vec3 maxPt = (transform.Rotation * m_VertexCoords[0]) + transform.Position;
		float maxDist = glm::dot(dir, maxPt);
		for (int i = 1; i < m_VertexCoords.size(); i++) {
			const glm::vec3 pt = (transform.Rotation * m_VertexCoords[i]) + transform.Position;
			const float dist = glm::dot(dir,pt);
			if (dist > maxDist) {
				maxDist = dist;
				maxPt = pt;
			}
		}

		glm::vec3 norm = glm::normalize(dir);
		norm *= bias;
		return maxPt + norm;
	}

	/*
	====================================================
	ShapeBox::InertiaTensor
	====================================================
	*/
	glm::mat3 PhysicalShapeBox::InertiaTensor() const {
		//inertiaTensor around origin
		glm::mat3 tensor = glm::mat3(0.0f);
		const float dx = m_bounds.maxs.x - m_bounds.mins.x;
		const float dy = m_bounds.maxs.y - m_bounds.mins.y;
		const float dz = m_bounds.maxs.z - m_bounds.mins.z;

		tensor[0][0] = (dy * dy + dz * dz) / 12.0f;
		tensor[1][1] = (dx * dx + dz * dz) / 12.0f;
		tensor[2][2] = (dx * dx + dy * dy) / 12.0f;

		//add modification around a different axis
		glm::vec3 cm = glm::vec3((m_bounds.maxs.x + m_bounds.mins.x) * 0.5f, (m_bounds.maxs.y + m_bounds.mins.y) * 0.5f, (m_bounds.maxs.z + m_bounds.mins.z) * 0.5f);

		const glm::vec3 R = glm::vec3(0, 0, 0) - cm;
		const float R2 = glm::length2(R);

		glm::mat3 patTensor;
		patTensor[0] = glm::vec3(R2 - R.x * R.x, R.x * R.y, R.x * R.z);
		patTensor[1] = glm::vec3(R.y * R.x, R2 - R.y * R.y, R.y * R.z);
		patTensor[2] = glm::vec3(R.z * R.x, R.z * R.y, R2 - R.z * R.z);
		tensor += patTensor;

		return tensor;
	}

	/*
	====================================================
	ShapeBox::GetBounds
	====================================================
	*/
	Bounds PhysicalShapeBox::GetBounds(const Transform& transform) const {
		glm::vec3 corners[8];

		corners[0] = glm::vec3(m_bounds.mins.x, m_bounds.mins.y, m_bounds.mins.z);
		corners[1] = glm::vec3(m_bounds.maxs.x, m_bounds.mins.y, m_bounds.mins.z);
		corners[2] = glm::vec3(m_bounds.mins.x, m_bounds.maxs.y, m_bounds.mins.z);
		corners[3] = glm::vec3(m_bounds.mins.x, m_bounds.mins.y, m_bounds.maxs.z);

		corners[4] = glm::vec3(m_bounds.maxs.x, m_bounds.maxs.y, m_bounds.maxs.z);
		corners[5] = glm::vec3(m_bounds.mins.x, m_bounds.maxs.y, m_bounds.maxs.z);
		corners[6] = glm::vec3(m_bounds.maxs.x, m_bounds.mins.y, m_bounds.maxs.z);
		corners[7] = glm::vec3(m_bounds.maxs.x, m_bounds.maxs.y, m_bounds.mins.z);

		Bounds bounds;
		for (int i = 0; i < 8; i++) {
			bounds.Expand((transform.Rotation * corners[i]) + transform.Position);
		}

		return bounds;
	}

	/*
	====================================================
	ShapeBox::FastestLinearSpeed
	====================================================
	*/
	float PhysicalShapeBox::FastestLinearSpeed(const  glm::vec3& angularVelocity, const  glm::vec3& dir) const {
		float maxSpeed = 0.0f;
		for (int i = 0; i < m_VertexCoords.size(); i++) {
			glm::vec3 r = m_VertexCoords[i] - m_CenterOfMass;
			glm::vec3 linearVelocity = glm::cross(angularVelocity, r);//perpendicular component of the angular speed
			float speed = glm::dot(dir, linearVelocity);//component of the perpendicular speed in direction of dir
			if (speed > maxSpeed) {
				maxSpeed = speed;
			}
		}

		return maxSpeed;
	}
}