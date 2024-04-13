#pragma once
#include "Fengshui/ECS/ECS.h"

namespace Fengshui
{
	class Scene;

	class Entity
	{
	public:
		Entity(const std::string& name = "GameEntity");

		~Entity();

		template<typename T>
		T& AddComponent()
		{
			GeneralManager::AddComponent<T>(m_EntityID, T{});
			return GeneralManager::GetComponent<T>(m_EntityID);
		}

		template<typename T>
		T& AddComponent(T arg)
		{
			GeneralManager::AddComponent<T>(m_EntityID, arg);
			return GeneralManager::GetComponent<T>(m_EntityID);// In case the component is already present, return the exisiting component instead
		}

		template<typename T>
		T& GetComponent()
		{
			return GeneralManager::GetComponent<T>(m_EntityID);
		}

		template<typename T>
		void RemoveComponent()
		{
			GeneralManager::RemoveComponent<T>(m_EntityID);
		}

		inline EntityID GetID() { return m_EntityID; }

		inline std::string& Name() { return GetComponent<Tag>().Name; }

	private:
		EntityID m_EntityID;
		WeakRef<Scene> m_Scene;
	};
}