#pragma once

#include "Fengshui/Core/Scene.h"

#include "Fengshui/ECS/Components/Components.h"

namespace Fengshui
{
	class Scene;

	class GameEntity
	{
	public:
		GameEntity() 
		{
			FS_ENGINE_ASSERT(false, "No scene specified");
		}

		GameEntity(Ref<Scene> scene, Ref<HierarchyComponent> parent = nullptr);

		static Ref<GameEntity> Create(Ref<Scene> scene, Ref<HierarchyComponent> parent = nullptr);

		~GameEntity();

		const uint32_t GetID() const { return m_EntityID; }
		std::vector<Ref<Component>> GetComponents() { return m_Components; }

		template <typename ReturnType, typename... Args>
		Ref<ReturnType> AddComponent(Args... args)
		//ReturnType* AddComponent()
		{
			Ref<ReturnType> newComp = std::make_shared<ReturnType>(args...);
			if (m_Scene->RegisterComponent(m_EntityID, newComp))
			{
				m_Components.emplace_back(newComp);
				return newComp;
			}
			FS_WARN("ADD COMPONENT FAILED");
			return nullptr;
		}

		template <typename T>
		Ref<T> GetComponent()
		{
			for (Ref<Component> comp : m_Components)
			{
				if (typeid(*comp) == typeid(T))
				{
					return std::dynamic_pointer_cast<T>(comp);
				}
			}
			FS_WARN("Component not found");
			return nullptr;
		}

		template <typename T>
		void RemoveComponent()
		{
			for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
			{
				Ref<T> element = std::dynamic_pointer_cast<T>(*it);
				if (element != nullptr)
				{
					m_Components.erase(it);
					m_Scene->RemoveComponent(m_EntityID, element);
					return;
				}
			}
			FS_WARN("Component not found");
		}

	private:
		uint32_t m_EntityID;
		std::vector<Ref<Component>> m_Components;
		Ref<Scene> m_Scene;
	};
}