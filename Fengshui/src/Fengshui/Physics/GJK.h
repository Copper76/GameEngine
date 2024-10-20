//
//	GJK.h
//
#pragma once
#include "Fengshui/Maths/Vector.h"
#include "Fengshui/Maths/Matrix.h"
#include "Fengshui/Maths/Bounds.h"
#include "Shapes.h"

#include "Fengshui/Physics/Helper.h"

namespace Fengshui
{
	bool GJK_DoesIntersect(const Collider& colliderA, const Transform& transA, const Collider& colliderB, const Transform& transB);
	bool GJK_DoesIntersect(const Collider& colliderA, const Transform& transA, const Collider& colliderB, const Transform& transB, const float bias, glm::vec3& ptOnA, glm::vec3& ptOnB);
	void GJK_ClosestPoints(const Collider& colliderA, const Transform& transA, const Collider& colliderB, const Transform& transB, glm::vec3& ptOnA, glm::vec3& ptOnB);
}
