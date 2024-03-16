#include "fspch.h"
#include "GameEntity.h"

namespace Fengshui
{
	GameEntity::GameEntity(Ref<Scene> scene, Ref<HierarchyComponent> parent)
	{
		m_Scene = scene;
	}

	Ref<GameEntity> GameEntity::Create(Ref<Scene> scene, Ref<HierarchyComponent> parent)
	{
		Ref<GameEntity> entity = std::make_shared<GameEntity>(scene, parent);
		entity->m_EntityID = entity->m_Scene->RegisterEntity(entity);
		auto hierarchy = entity->AddComponent<HierarchyComponent>();//Every entity should have a hierarchy
		hierarchy->SetParent(parent);
		return entity;
	}

	GameEntity::~GameEntity()
	{
		m_Components.clear();
	}
}