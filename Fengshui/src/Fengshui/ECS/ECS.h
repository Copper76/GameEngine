#pragma once
#include "Fengshui/ECS/Components.h"

#include <bitset>
#include <queue>
#include <set>

#include <glm/glm.hpp>

namespace Fengshui
{
#pragma region Settings
	//Entity Settings
	using Entity = uint32_t;

	const Entity MAX_ENTITIES = 5000;

	//Component Settings
	using ComponentType = uint8_t;

	const ComponentType MAX_COMPONENTS = 32;

	using Signature = std::bitset<MAX_COMPONENTS>;
#pragma endregion

#pragma region Data Structure
	class IComponentArray
	{
	public:
		virtual ~IComponentArray() = default;
		virtual void OnEntityDestroyed(Entity entity) = 0;
	};

	template<typename T>
	class ComponentArray : public IComponentArray
	{
	public:
		void InsertData(Entity entity, T component)
		{
			if (m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end()) return;//don't add component more than once

			size_t newIndex = m_Size;
			m_EntityToIndexMap[entity] = newIndex;
			m_IndexToEntityMap[newIndex] = entity;
			m_ComponentArray[newIndex] = component;
			++m_Size;
		}

		void RemoveData(Entity entity)
		{
			if (m_EntityToIndexMap.find(entity) == m_EntityToIndexMap.end()) return;//the component doesn't exist

			size_t removeIndex = m_EntityToIndexMap[entity];
			size_t lastIndex = m_Size - 1;
			m_ComponentArray[removeIndex] = m_ComponentArray[lastIndex];

			Entity lastEntity = m_IndexToEntityMap[lastIndex];
			m_EntityToIndexMap[lastEntity] = removeIndex;
			m_IndexToEntityMap[removeIndex] = lastEntity;

			m_EntityToIndexMap.erase(entity);
			m_IndexToEntityMap.erase(lastIndex);

			--m_Size;
		}

		T& GetData(Entity entity)
		{
			FS_ENGINE_ASSERT(m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end(), "Component doesn't exist");//the component doesn't exist

			return m_ComponentArray[m_EntityToIndexMap[entity]];
		}

		void OnEntityDestroyed(Entity entity) override
		{
			RemoveData(entity);//delegate the checking to later RemoveData
		}

	private:
		std::array<T, MAX_ENTITIES> m_ComponentArray;
		std::unordered_map<Entity, size_t> m_EntityToIndexMap;
		std::unordered_map<size_t, Entity> m_IndexToEntityMap;

		size_t m_Size;
	};

	class System
	{
	public:
		std::set<Entity> m_Entities;
	};

#pragma endregion

#pragma region Managers
	class EntityManager
	{
	public:
		EntityManager()
		{
			for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
			{
				m_AvailableEntities.push(entity);
			}
		}

		Entity Create()
		{
			FS_ENGINE_ASSERT(m_UsedEntityCount < MAX_ENTITIES, "Entity exceeded limit");

			Entity id = m_AvailableEntities.front();
			m_AvailableEntities.pop();
			++m_UsedEntityCount;

			return id;
		}

		void Destroy(Entity entity)
		{
			FS_ENGINE_ASSERT(entity < MAX_ENTITIES && entity > -1, "Entity out of range");

			m_Signatures[entity].reset();
		}

		void SetSignature(Entity entity, Signature signature)
		{
			FS_ENGINE_ASSERT(entity < MAX_ENTITIES && entity > -1, "Entity out of range");
			m_Signatures[entity] = signature;
		}

		Signature GetSignature(Entity entity)
		{
			FS_ENGINE_ASSERT(entity < MAX_ENTITIES && entity > -1, "Entity out of range");
			return m_Signatures[entity];
		}

	private:
		std::queue<Entity> m_AvailableEntities;

		std::array<Signature, MAX_ENTITIES> m_Signatures;

		uint32_t m_UsedEntityCount = 0;
	};

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
		void AddComponent(Entity entity, T component)
		{
			GetComponentArray<T>()->InsertData(entity, component);
		}

		template<typename T>
		void RemoveComponent(Entity entity)
		{
			GetComponentArray<T>()->RemoveData(entity);
		}

		template<typename T>
		T& GetComponent(Entity entity)
		{
			return GetComponentArray<T>()->GetData(entity);
		}

		void OnEntityDestroyed(Entity entity)
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

	class SystemManager
	{
	public:
		template<typename T>
		Ref<T> RegisterSystem()
		{
			const char* typeName = typeid(T).name();
			if (m_Systems.find(typeName) != m_Systems.end()) return std::static_pointer_cast<T>(m_Systems.find(typeName)->second);//System already registered

			auto system = std::make_shared<T>();
			m_Systems.insert({ typeName, system });
			return system;
		}

		template<typename T>
		void SetSignature(Signature signature)
		{
			const char* typeName = typeid(T).name();
			FS_ENGINE_ASSERT(m_Systems.find(typeName) != m_Systems.end(), "System was never registered");//Dangerous function

			m_Signatures.insert({ typeName, signature });
		}

		void OnEntityDestroyed(Entity entity)
		{
			for (auto& pair : m_Systems)
			{
				auto const& system = pair.second;
				system->m_Entities.erase(entity);
			}
		}

		void OnEntitySignatureChanged(Entity entity, Signature entitySignature) {
			for (auto const& pair : m_Systems)
			{
				auto const& type = pair.first;
				auto const& system = pair.second;
				auto const& systemSignature = m_Signatures[type];

				if ((entitySignature & systemSignature) == systemSignature)
				{
					system->m_Entities.insert(entity);//add if matching
				}
				else
				{
					system->m_Entities.erase(entity);//remove if no longer matching
				}
			}
		}

	private:
		std::unordered_map<const char*, Signature> m_Signatures;
		std::unordered_map<const char*, Ref<System>> m_Systems;
	};

	class GeneralManager
	{
	public:
		static void Init()
		{
		}

		static Entity CreateEntity()
		{
			return m_EntityManager->Create();
		}

		static void DestroyEntity(Entity entity)
		{
			m_EntityManager->Destroy(entity);
			m_ComponentManager->OnEntityDestroyed(entity);
			m_SystemManager->OnEntityDestroyed(entity);
		}

		template<typename T>
		static void RegisterComponent()
		{
			m_ComponentManager->RegisterComponent<T>();
		}

		template<typename T>
		static void AddComponent(Entity entity, T component)
		{
			m_ComponentManager->AddComponent<T>(entity, component);

			auto signature = m_EntityManager->GetSignature(entity);
			signature.set(m_ComponentManager->GetComponentType<T>(), true);
			m_EntityManager->SetSignature(entity, signature);

			m_SystemManager->OnEntitySignatureChanged(entity, signature);
		}

		template<typename T>
		static void RemoveComponent(Entity entity)
		{
			m_ComponentManager->RemoveComponent<T>(entity);

			auto signature = m_EntityManager->GetSignature(entity);
			signature.set(m_ComponentManager->GetComponentType<T>(), false);
			m_EntityManager->SetSignature(entity, signature);

			m_SystemManager->OnEntitySignatureChanged(entity, signature);
		}

		template<typename T>
		static T& GetComponent(Entity entity)
		{
			return m_ComponentManager->GetComponent<T>(entity);
		}

		template<typename T>
		static ComponentType GetComponentType()
		{
			return m_ComponentManager->GetComponentType<T>();
		}

		template<typename T>
		static Ref<T> RegisterSystem()
		{
			return m_SystemManager->RegisterSystem<T>();
		}

		template<typename T>
		static void SetSystemSignature(Signature signature)
		{
			m_SystemManager->SetSignature<T>(signature);
		}

	private:
		static Ref<EntityManager> m_EntityManager;
		static Ref<ComponentManager> m_ComponentManager;
		static Ref<SystemManager> m_SystemManager;
	};
#pragma endregion

}