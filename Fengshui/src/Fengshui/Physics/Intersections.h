//
//	Intersections.h
//
#pragma once
#include "Contact.h"
#include "Fengshui/Physics/Helper.h"

namespace Fengshui
{
	bool Intersect(const CollisionPair pair, const Rigidbody& bodyA, const Rigidbody& bodyB, const Transform& transA, const Transform& transB, const Collider& colliderA, const Collider& colliderB, contact_t& contact);
	bool Intersect(const CollisionPair pair, const float dt, contact_t& contact);
}