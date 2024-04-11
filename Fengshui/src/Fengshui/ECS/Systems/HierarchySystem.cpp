#include "fspch.h"
#include "Fengshui/ECS/Systems/HierarchySystem.h"

namespace Fengshui
{
	void HierarchySystem::Destroy(EntityID entity)
	{
		auto& related = GeneralManager::GetComponent<Hierarchy>(entity);
		for (EntityID e : related.Children)
		{
			DestroyInternal(e);
		}
		auto& parent = GeneralManager::GetComponent<Hierarchy>(related.Parent);
		parent.Children.erase(entity);
		GeneralManager::DestroyEntity(entity);
	}

	void HierarchySystem::DestroyInternal(EntityID entity)
	{
		auto& related = GeneralManager::GetComponent<Hierarchy>(entity);
		for (EntityID e : related.Children)
		{
			DestroyInternal(e);
		}
		GeneralManager::DestroyEntity(entity);
	}
}