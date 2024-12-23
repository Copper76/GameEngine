//
//	Broadphase.h
//
#pragma once
#include <vector>

#include "Fengshui/ECS/ECS.h"
#include "Fengshui/Physics/Helper.h"

namespace Fengshui
{
	void BroadPhase(const std::set<EntityID> entities, const int num, std::vector<CollisionPair>& finalPairs, const float dt_sec);
}