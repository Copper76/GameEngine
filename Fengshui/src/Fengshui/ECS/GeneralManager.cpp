#include "fspch.h"
#include "Fengshui/ECS/GeneralManager.h"

namespace Fengshui
{
	Scope<GeneralManager> GeneralManager::m_Instance = std::make_unique<GeneralManager>();
}