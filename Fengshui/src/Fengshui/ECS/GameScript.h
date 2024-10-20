#pragma once

#include "ECS.h"
#include <unordered_set>

namespace Fengshui
{
	enum class UpdateGroup
	{
		PREPHYSICS, POSTPHYSICS, UPDATESTART, UPDATEEND
	};

	class GameScript : public ScriptComponent
	{
	protected:
		std::unordered_map<UpdateGroup, std::unordered_set<void(const float)>> m_UpdateFunctions;
	};
}