#pragma once
#include "Fengshui/ECS/ECS.h"

namespace Fengshui
{
	class ComponentManager
	{
	public:
		template<typename T>
		void RegisterComponent()
		{
			const char* typeName = typeid(T).name();

			if (m_ComponentTypes.find(typeName) != m_ComponentTypes.end()) return;//Component already registered

			m_ComponentTypes.insert({ typeName, m_NextComponentType });

			m_ComponentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });

			++m_NextComponentType;
		}

		template<typename T>
		ComponentType GetComponentType()
		{
			const char* typeName = typeid(T).name();

			FS_ENGINE_ASSERT(m_ComponentTypes.find(typeName) != m_ComponentTypes.end(), "Component type was never registered");//Dangerous function

			return m_ComponentTypes[typeName];
		}

		template<typename T>
		void AddComponent(EntityID entity, T component)
		{
			GetComponentArray<T>()->InsertData(entity, component);
		}

		template<typename T>
		void RemoveComponent(EntityID entity)
		{
			GetComponentArray<T>()->RemoveData(entity);
		}

		template<typename T>
		bool HasComponent(EntityID entity)
		{
			return GetComponentArray<T>()->HasData(entity);
		}

		template<typename T>
		T& GetComponent(EntityID entity)
		{
			return GetComponentArray<T>()->GetData(entity);
		}

		void OnEntityDestroyed(EntityID entity)
		{
			for (auto const& pair : m_ComponentArrays)
			{
				auto const& component = pair.second;

				component->OnEntityDestroyed(entity);
			}
		}

	private:
		std::unordered_map<const char*, ComponentType> m_ComponentTypes;

		std::unordered_map<const char*, Ref<IComponentArray>> m_ComponentArrays;

		ComponentType m_NextComponentType = 0;

		template<typename T>
		Ref<ComponentArray<T>> GetComponentArray()
		{
			const char* typeName = typeid(T).name();
			FS_ENGINE_ASSERT(m_ComponentTypes.find(typeName) != m_ComponentTypes.end(), "Component type was never registered");//Dangerous function

			return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[typeName]);
		}
	};
}