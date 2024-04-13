#include "fspch.h"
#include "HierarchyDisplaySystem.h"

namespace Fengshui
{
	std::vector<std::string> HierarchyDisplaySystem::GetEntityTags()
	{
		std::vector<std::string> entityTags = std::vector<std::string>();
		entityTags.reserve(m_Entities.size());
		for (EntityID e : m_Entities)
		{
			entityTags.emplace_back(GeneralManager::GetComponent<Tag>(e).Name);
		}
		return entityTags;
	}
}