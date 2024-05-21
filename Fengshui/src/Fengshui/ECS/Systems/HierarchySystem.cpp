#include "fspch.h"
#include "Fengshui/ECS/Systems/HierarchySystem.h"

namespace Fengshui
{
	void HierarchySystem::Destroy(EntityID entity)
	{
		auto& related = GeneralManager::GetComponent<Hierarchy>(entity);
		std::set<EntityID> children = related.Children;
		for (EntityID e : children)
		{
			LinkChild(e, related.Parent);
		}
		auto& parent = GeneralManager::GetComponent<Hierarchy>(related.Parent);
		parent.Children.erase(entity);
		GeneralManager::DestroyEntity(entity);
	}

	void HierarchySystem::SetParent(EntityID entity, EntityID parent)
	{
		FS_ENGINE_ASSERT(entity != 0, "Root Node cannot be a child of anything");
		auto& hierarchy = GeneralManager::GetComponent<Hierarchy>(entity);
		GeneralManager::GetComponent<Hierarchy>(hierarchy.Parent).Children.erase(entity);
		hierarchy.Parent = parent;
		GeneralManager::GetComponent<Hierarchy>(parent).Children.insert(entity);
	}

	void HierarchySystem::LinkChild(EntityID entity, EntityID parent)
	{
		GeneralManager::GetComponent<Hierarchy>(entity).Parent = parent;
		GeneralManager::GetComponent<Hierarchy>(parent).Children.insert(entity);
	}
}