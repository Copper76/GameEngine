#include "fspch.h"
#include "Fengshui/ECS/Entity.h"
#include "Fengshui/Core/Scene.h"

namespace Fengshui
{

	Entity::Entity(const std::string& name, Ref<Entity> parent)
	{
		m_Scene = GeneralManager::GetActiveScene();
		m_EntityID = GeneralManager::CreateEntity();
		AddComponent<Tag>(name);
		AddComponent<Hierarchy>();
		if (m_EntityID != 0)
		{
			if (parent != nullptr)
			{
				SetParent(parent);
			}
			else
			{
				SetParent(0);
			}
		}

		GeneralManager::GetActiveScene()->AddEntity(std::shared_ptr<Entity>(this, [](Entity*) {}));
	}

	Entity::~Entity()
	{
		if (m_Scene.lock())
		{
			GeneralManager::GetSystem<HierarchySystem>()->Destroy(m_EntityID);
		}
	}

	void Entity::SetParent(EntityID parentID)
	{
		GeneralManager::GetSystem<HierarchySystem>()->SetParent(m_EntityID, parentID);
	}

	void Entity::SetParent(Ref<Entity> parent)
	{
		SetParent(parent->GetID());
	}

	void Entity::SetParent(WeakRef<Entity> parent)
	{
		SetParent(parent.lock()->GetID());
	}

	void Entity::AddChild(Ref<Entity> child)
	{
		GeneralManager::GetSystem<HierarchySystem>()->SetParent(child->GetID(), m_EntityID);
	}

	void Entity::AddChild(WeakRef<Entity> child)
	{
		GeneralManager::GetSystem<HierarchySystem>()->SetParent(child.lock()->GetID(), m_EntityID);
	}
}
