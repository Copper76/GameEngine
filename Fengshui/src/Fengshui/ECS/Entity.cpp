#include "fspch.h"
#include "Fengshui/ECS/Entity.h"

namespace Fengshui
{

	Entity::Entity(const std::string& name)
	{
		m_Scene = GeneralManager::GetActiveScene();
		m_EntityID = GeneralManager::CreateEntity();
		AddComponent<Tag>(name);
		AddComponent<Hierarchy>();
	}

	Entity::~Entity()
	{
		GeneralManager::DestroyEntity(m_EntityID);
	}
}
