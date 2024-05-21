//
//	GJK.h
//
#pragma once
#include "../Math/Vector.h"
#include "../Math/Quat.h"
#include "../Math/Matrix.h"
#include "../Math/Bounds.h"
#include "Shapes.h"

#include "Fengshui/Physics/Helper.h"

namespace Fengshui
{
	bool GJK_DoesIntersect(const Collider colliderA, const Transform transA, const Collider colliderB, const Transform transB);
	bool GJK_DoesIntersect(const Collider colliderA, const Transform transA, const Collider colliderB, const Transform transB, const float bias, glm::vec3& ptOnA, glm::vec3& ptOnB);
	void GJK_ClosestPoints(const Collider colliderA, const Transform transA, const Collider colliderB, const Transform transB, glm::vec3& ptOnA, glm::vec3& ptOnB);
}
