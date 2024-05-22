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
	glm::vec3 ShapeSphere::Support(const glm::vec3& dir, const Transform transform, const Collider collider, const float bias) const {
		return (transform.Position + collider.Offset + transform.Rotation * transform.Scale * collider.Size * dir * (m_radius + bias));
	}

	/*
	====================================================
	ShapeSphere::InertiaTensor
	====================================================
	*/
	glm::mat3 ShapeSphere::InertiaTensor(const Transform transform, const Collider collider) const {
		glm::mat3 tensor = glm::mat3(0.0f);
		tensor[0][0] = 2.0f * m_radius * m_radius / 5.0f * transform.Scale.x * collider.Size.x + collider.Offset.x;
		tensor[1][1] = 2.0f * m_radius * m_radius / 5.0f * transform.Scale.y * collider.Size.y + collider.Offset.y;
		tensor[2][2] = 2.0f * m_radius * m_radius / 5.0f * transform.Scale.z * collider.Size.z + collider.Offset.z;
		return (glm::mat3_cast(transform.Rotation) * tensor);
	}

	/*
	====================================================
	ShapeSphere::GetBounds
	====================================================
	*/
	Bounds ShapeSphere::GetBounds(const Transform transform, const Collider collider) const {
		Bounds tmp;
		tmp.mins = (transform.Rotation * glm::vec3(-m_radius)) * transform.Scale * collider.Size + transform.Position + collider.Offset;
		tmp.maxs = (transform.Rotation * glm::vec3(m_radius)) * transform.Scale * collider.Size + transform.Position;
		return tmp;
	}

	/*
	====================================================
	ShapeSphere::GetBounds
	====================================================
	*/
	Bounds ShapeSphere::GetBounds(const glm::vec3 scale, const Collider collider) const {
		Bounds tmp;
		tmp.mins = glm::vec3(-m_radius) * scale * collider.Size + collider.Offset;
		tmp.maxs = glm::vec3(m_radius) * scale * collider.Size + collider.Offset;
		return tmp;
	}
}