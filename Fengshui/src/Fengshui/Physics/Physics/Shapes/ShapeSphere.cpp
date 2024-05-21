//
//  ShapeSphere.cpp
//
#include "fspch.h"

#include "Fengshui/ECS/Components.h"

#include "ShapeSphere.h"
#include <glm/gtx/norm.hpp>
#include <glm/gtc/quaternion.hpp>

///*
//========================================================================================================
//
//ShapeSphere
//
//========================================================================================================
//*/
//
///*
//====================================================
//ShapeSphere::Support
//====================================================
//*/
//Vec3 ShapeSphere::Support(const Vec3& dir, const Vec3& pos, const Quat& orient, const float bias) const {
//	return (pos + dir * (m_radius + bias));
//}
//
///*
//====================================================
//ShapeSphere::InertiaTensor
//====================================================
//*/
//Mat3 ShapeSphere::InertiaTensor() const {
//	Mat3 tensor;
//	tensor.Zero();
//	tensor.rows[0][0] = 2.0f * m_radius * m_radius / 5.0f;
//	tensor.rows[1][1] = 2.0f * m_radius * m_radius / 5.0f;
//	tensor.rows[2][2] = 2.0f * m_radius * m_radius / 5.0f;
//	return tensor;
//}
//
///*
//====================================================
//ShapeSphere::GetBounds
//====================================================
//*/
//Bounds ShapeSphere::GetBounds(const Vec3& pos, const Quat& orient) const {
//	Bounds tmp;
//	tmp.mins = Vec3(-m_radius) + pos;
//	tmp.maxs = Vec3(m_radius) + pos;
//	return tmp;
//}
//
///*
//====================================================
//ShapeSphere::GetBounds
//====================================================
//*/
//Bounds ShapeSphere::GetBounds() const {
//	Bounds tmp;
//	tmp.mins = Vec3(-m_radius);
//	tmp.maxs = Vec3(m_radius);
//	return tmp;
//}

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
	glm::mat3 ShapeSphere::InertiaTensor() const {
		glm::mat3 tensor = glm::mat3(0.0f);
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
	Bounds ShapeSphere::GetBounds(const Transform transform) const {
		Bounds tmp;
		tmp.mins = glm::vec3(-m_radius) * transform.Scale + transform.Position;
		tmp.maxs = glm::vec3(m_radius) * transform.Scale + transform.Position;
		return tmp;
	}

	/*
	====================================================
	ShapeSphere::GetBounds
	====================================================
	*/
	Bounds ShapeSphere::GetBounds() const {
		Bounds tmp;
		tmp.mins = glm::vec3(-m_radius);
		tmp.maxs = glm::vec3(m_radius);
		return tmp;
	}
}