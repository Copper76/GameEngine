#include "fspch.h"
#include "GameEntity.h"

namespace Fengshui
{
	GameEntity::GameEntity(Ref<Scene> scene, Ref<HierarchyComponent> parent)
	{
		m_Scene = scene;
		m_EntityID = m_Scene->RegisterEntity(this);
		auto hierarchy = AddComponent<HierarchyComponent>();//Every entity should have a hierarchy
		hierarchy->SetParent(parent);
	}

	GameEntity::~GameEntity()
	{
		for (Ref<Component> comp : m_Components)
		{
			m_Scene->RemoveComponent(comp, m_EntityID);
		}
		m_Components.clear();
	}
}