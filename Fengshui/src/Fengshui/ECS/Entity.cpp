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
	}

	Entity::~Entity()
	{
		GeneralManager::DestroyEntity(m_EntityID);
	}

	void Entity::SetParent(EntityID parentID)
	{
		GeneralManager::GetActiveScene()->GetHierarchySystem()->SetParent(m_EntityID, parentID);
	}

	void Entity::SetParent(Ref<Entity> parent)
	{
		GeneralManager::GetActiveScene()->GetHierarchySystem()->SetParent(m_EntityID, parent->GetID());
	}

	void Entity::AddChild(Ref<Entity> child)
	{
		GeneralManager::GetActiveScene()->GetHierarchySystem()->AddChild(m_EntityID, child->GetID());
	}
}
