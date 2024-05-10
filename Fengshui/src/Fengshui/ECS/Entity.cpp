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
		//if (m_Scene.lock())
		if (m_Scene.lock())
		{
			GeneralManager::GetSystem<HierarchySystem>()->Destroy(m_EntityID);
			//m_Scene.lock()->GetHierarchySystem()->Destroy(m_EntityID);
		}
	}

	void Entity::SetParent(EntityID parentID)
	{
		//m_Scene.lock()->GetHierarchySystem()->SetParent(m_EntityID, parentID);
		GeneralManager::GetSystem<HierarchySystem>()->SetParent(m_EntityID, parentID);
	}

	void Entity::SetParent(Ref<Entity> parent)
	{
		//m_Scene.lock()->GetHierarchySystem()->SetParent(m_EntityID, parent->GetID());
		SetParent(parent->GetID());
	}

	void Entity::AddChild(Ref<Entity> child)
	{
		//m_Scene.lock()->GetHierarchySystem()->AddChild(m_EntityID, child->GetID());
		GeneralManager::GetSystem<HierarchySystem>()->SetParent(child->GetID(), m_EntityID);
	}
}
