#include "fspch.h"
#include "GameEntity.h"

namespace Fengshui
{
	void GameEntity::AddComponent(Component* component)
	{
		m_Components.emplace_back(component);
	}

	void GameEntity::RemoveComponent(Component* component)
	{
		auto it = std::find(m_Components.begin(), m_Components.end(), component);
		if (it != m_Components.end())
		{
			//component->Detach();
			m_Components.erase(it);
		}
	}
}