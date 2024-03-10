#pragma once

#include "Fengshui/ECS/Components/BaseComponent.h"
#include "Components/TransformComponent.h"

namespace Fengshui
{
	class GameEntity
	{
	public:
		GameEntity(uint32_t id) : m_EntityID(id)
		{

		}

		const uint32_t GetEntityID() const { return m_EntityID; }
		std::vector<Component*> GetComponents() { return m_Components; }

		void AddComponent(Component* component);
		void RemoveComponent(Component* component);

	private:
		uint32_t m_EntityID;
		std::vector<Component*> m_Components;
	};
}

