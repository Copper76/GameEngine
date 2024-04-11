#include "fspch.h"
#include "Fengshui/ECS/Entity.h"

namespace Fengshui
{

	Entity::Entity(const std::string& name)
	{
		entityID = GeneralManager::CreateEntity();
		AddComponent<Tag>(name);
		AddComponent<Hierarchy>();
	}
}
