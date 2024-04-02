#include "fspch.h"
#include "Fengshui/ECS/ECS.h"

namespace Fengshui
{
	Ref<EntityManager> GeneralManager::m_EntityManager = std::make_shared<EntityManager>();
	Ref<ComponentManager> GeneralManager::m_ComponentManager = std::make_shared<ComponentManager>();
	Ref<SystemManager> GeneralManager::m_SystemManager = std::make_shared<SystemManager>();
}