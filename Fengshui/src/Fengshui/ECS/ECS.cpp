#include "fspch.h"
#include "Fengshui/ECS/ECS.h"

namespace Fengshui
{
	Scope<GeneralManager> GeneralManager::m_Instance = std::make_unique<GeneralManager>();
}