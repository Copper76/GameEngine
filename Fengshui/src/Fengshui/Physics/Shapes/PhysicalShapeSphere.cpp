//
//  ShapeSphere.cpp
//
#include "fspch.h"

#include "Fengshui/ECS/Components.h"

#include "PhysicalShapeSphere.h"
#include <glm/gtx/norm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Fengshui
{
	/*
========================================================================================================

ShapeSphere

========================================================================================================
*/

/*
====================================================
ShapeSphere::Support
====================================================
*/
	glm::vec3 PhysicalShapeSphere::Support(const glm::vec3& dir, const Transform transform, const float bias) const {
		return (transform.Position + transform.Rotation * dir * (m_radius + bias));
	}

	/*
	====================================================
	ShapeSphere::InertiaTensor
	====================================================
	*/
	glm::mat3 PhysicalShapeSphere::InertiaTensor() const {
		glm::mat3 tensor = glm::mat3(0.0f);
		//tensor[0][0] = 2.0f * m_radius.x * m_radius.x / 5.0f;
		//tensor[1][1] = 2.0f * m_radius.y * m_radius.y / 5.0f;
		//tensor[2][2] = 2.0f * m_radius.z * m_radius.z / 5.0f;

		tensor[0][0] = 2.0f * m_radius * m_radius / 5.0f;
		tensor[1][1] = 2.0f * m_radius * m_radius / 5.0f;
		tensor[2][2] = 2.0f * m_radius * m_radius / 5.0f;
		return tensor;
	}

	/*
	====================================================
	ShapeSphere::GetBounds
	====================================================
	*/
	Bounds PhysicalShapeSphere::GetBounds(const Transform transform) const {
		Bounds tmp;
		//tmp.mins = (transform.Rotation * (-m_radius)) + transform.Position;
		//tmp.maxs = (transform.Rotation * m_radius) + transform.Position;
		tmp.mins = (transform.Rotation * glm::vec3(-m_radius)) + transform.Position;
		tmp.maxs = (transform.Rotation * glm::vec3(m_radius)) + transform.Position;
		return tmp;
	}
}