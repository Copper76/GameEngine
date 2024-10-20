#pragma once
#include "Fengshui/ECS/ECS.h"
#include "Fengshui/ECS/GeneralManager.h"

namespace Fengshui
{
	class Scene;

	class Entity : public std::enable_shared_from_this<Entity>
	{
	public:
		Entity(const std::string& name = "GameEntity", Ref<Entity> parent = nullptr);

		~Entity();

		template<typename T>
		T& AddComponent()
		{
			GeneralManager::AddComponent<T>(m_EntityID, T());
			return GeneralManager::GetComponent<T>(m_EntityID);
		}

		template<typename T>
		T& AddComponent(T arg)
		{
			GeneralManager::AddComponent<T>(m_EntityID, arg);
			return GeneralManager::GetComponent<T>(m_EntityID);// In case the component is already present, return the exisiting component instead
		}

		template<typename T>
		bool HasComponent()
		{
			return GeneralManager::HasComponent<T>(m_EntityID);
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

		void SetParent(Ref<Entity> parent);

		void SetParent(WeakRef<Entity> parent);

		void AddChild(Ref<Entity> child);

		void AddChild(WeakRef<Entity> child);

		inline EntityID GetID() { return m_EntityID; }

		inline std::string& Name() { return GetComponent<Tag>().Name; }

		operator EntityID() const { return m_EntityID; }

	private:
		void SetParent(EntityID parentID);

	private:
		EntityID m_EntityID;

		WeakRef<Scene> m_Scene;
	};
}