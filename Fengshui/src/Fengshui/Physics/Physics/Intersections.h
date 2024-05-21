//
//	Intersections.h
//
#pragma once
#include "Contact.h"
#include "Fengshui/Physics/Helper.h"

//bool Intersect( Body * bodyA, Body * bodyB, contact_t & contact );
//bool Intersect( Body * bodyA, Body * bodyB, const float dt, contact_t & contact );

namespace Fengshui
{
	bool Intersect(const collisionPair_t pair, contact_t& contact);
	bool Intersect(const collisionPair_t pair, const float dt, contact_t& contact);
}