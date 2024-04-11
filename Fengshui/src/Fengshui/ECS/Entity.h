#pragma once
#include "Fengshui/ECS/ECS.h"

namespace Fengshui
{
	class Entity
	{
	public:
		Entity(const std::string& name = "GameEntity");

		~Entity() = default;

		template<typename T>
		T& AddComponent()
		{
			GeneralManager::AddComponent<T>(entityID, T{});
			return GeneralManager::GetComponent<T>(entityID);
		}

		template<typename T>
		T& AddComponent(T arg)
		{
			GeneralManager::AddComponent<T>(entityID, arg);
			return GeneralManager::GetComponent<T>(entityID);// In case the component is already present, return the exisiting component instead
		}

		template<typename T>
		T& GetComponent()
		{
			return GeneralManager::GetComponent<T>(entityID);
		}

		template<typename T>
		void RemoveComponent()
		{
			GeneralManager::RemoveComponent<T>(entityID);
		}

		inline EntityID GetID() { return entityID; }

		inline std::string& Name() { return GetComponent<Tag>().Name; }

	private:
		EntityID entityID;
	};
}