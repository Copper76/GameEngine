//
//  ShapeSphere.cpp
//
#include "fspch.h"

#include "Fengshui/ECS/Components.h"

#include "ShapeSphere.h"
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
	glm::vec3 ShapeSphere::Support(const glm::vec3& dir, const Transform transform, const float bias) const {
		return (transform.Position + transform.Rotation * transform.Scale * dir * (m_radius + bias));
	}

	/*
	====================================================
	ShapeSphere::InertiaTensor
	====================================================
	*/
	glm::mat3 ShapeSphere::InertiaTensor(const Transform transform) const {
		glm::mat3 tensor = glm::mat3(0.0f);
		tensor[0][0] = 2.0f * m_radius * m_radius / 5.0f * transform.Scale.x;
		tensor[1][1] = 2.0f * m_radius * m_radius / 5.0f * transform.Scale.y;
		tensor[2][2] = 2.0f * m_radius * m_radius / 5.0f * transform.Scale.z;
		return (glm::mat3_cast(transform.Rotation) * tensor);
	}

	/*
	====================================================
	ShapeSphere::GetBounds
	====================================================
	*/
	Bounds ShapeSphere::GetBounds(const Transform transform) const {
		Bounds tmp;
		tmp.mins = (transform.Rotation * glm::vec3(-m_radius)) * transform.Scale + transform.Position;
		tmp.maxs = (transform.Rotation * glm::vec3(m_radius)) * transform.Scale + transform.Position;
		return tmp;
	}

	/*
	====================================================
	ShapeSphere::GetBounds
	====================================================
	*/
	Bounds ShapeSphere::GetBounds(const glm::vec3 scale) const {
		Bounds tmp;
		tmp.mins = glm::vec3(-m_radius) * scale;
		tmp.maxs = glm::vec3(m_radius) * scale;
		return tmp;
	}
}