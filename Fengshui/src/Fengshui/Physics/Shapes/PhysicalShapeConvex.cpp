//
//  ShapeConvex.cpp
//
#include "fspch.h"

#include "Fengshui/ECS/Components.h"

#include "PhysicalShapeConvex.h"
#include <glm/gtx/norm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Fengshui
{
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
	void PhysicalShapeConvex::Build(const glm::vec3* pts, const int num) {
		m_points.clear();
		m_points.reserve(num);
		for (int i = 0; i < num; i++) {
			m_points.push_back(pts[i]);
		}

		//Create the hull
		std::vector<glm::vec3> hullPoints;
		std::vector<Triangle> hullTriangles;
		Geometry::BuildConvexHull(m_points, hullPoints, hullTriangles);
		m_points = hullPoints;

		//Expand the bounds
		m_bounds.Clear();
		m_bounds.Expand(m_points.data(), (int)m_points.size());
		m_centerOfMass = Geometry::CalculateCenterOfMassTetrahedron(hullPoints, hullTriangles);
		m_inertiaTensor = Geometry::CalculateInertiaTensorTetrahedron(hullPoints, hullTriangles, m_centerOfMass);
	}

	void PhysicalShapeConvex::Build(const std::vector<glm::vec3> pts, const std::vector<Triangle> tris) {
		m_points = pts;

		//Expand the bounds
		m_bounds.Clear();
		m_bounds.Expand(m_points.data(), (int)m_points.size());
		m_centerOfMass = Geometry::CalculateCenterOfMassTetrahedron(pts, tris);
		m_inertiaTensor = Geometry::CalculateInertiaTensorTetrahedron(pts, tris, m_centerOfMass);
	}

	/*
	====================================================
	ShapeConvex::Support
	====================================================
	*/
	glm::vec3 PhysicalShapeConvex::Support(const glm::vec3& dir, const Transform transform, const float bias) const {
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

	glm::mat3 PhysicalShapeConvex::InertiaTensor(const Transform transform) const
	{
		return m_inertiaTensor;
	}

	/*
	====================================================
	ShapeConvex::GetBounds
	====================================================
	*/
	Bounds PhysicalShapeConvex::GetBounds(const Transform transform) const {
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

	Bounds PhysicalShapeConvex::GetBounds(const glm::vec3 scale) const
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
	float PhysicalShapeConvex::FastestLinearSpeed(const glm::vec3& angularVelocity, const glm::vec3& dir) const {
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