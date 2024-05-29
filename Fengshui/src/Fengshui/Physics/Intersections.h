//
//	Intersections.h
//
#pragma once
#include "Contact.h"
#include "Fengshui/Physics/Helper.h"

namespace Fengshui
{
	bool Intersect(const CollisionPair pair, contact_t& contact);
	bool Intersect(const CollisionPair pair, const float dt, contact_t& contact);
}