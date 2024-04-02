#include "fspch.h"
#include "Fengshui/ECS/Systems/HierarchySystem.h"

namespace Fengshui
{
	void HierarchySystem::Destroy(Entity entity)
	{
		auto& related = GeneralManager::GetComponent<Hierarchy>(entity);
		for (Entity e : related.Children)
		{
			DestroyInternal(e);
		}
		auto& parent = GeneralManager::GetComponent<Hierarchy>(related.Parent);
		parent.Children.erase(entity);
		GeneralManager::DestroyEntity(entity);
	}

	void HierarchySystem::DestroyInternal(Entity entity)
	{
		auto& related = GeneralManager::GetComponent<Hierarchy>(entity);
		for (Entity e : related.Children)
		{
			DestroyInternal(e);
		}
		GeneralManager::DestroyEntity(entity);
	}
}