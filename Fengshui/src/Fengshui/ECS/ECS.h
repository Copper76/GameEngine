#pragma once
#include "Fengshui/ECS/Components.h"

#include <bitset>
#include <queue>
#include <set>
#include <array>

#include <glm/glm.hpp>

namespace Fengshui
{
#pragma region Settings
	//Entity Settings
	using EntityID = uint32_t;

	const EntityID MAX_ENTITIES = 5000;

	//Component Settings
	using ComponentType = uint8_t;

	const ComponentType MAX_COMPONENTS = 32;

	using Signature = std::bitset<MAX_COMPONENTS>;

	class Scene;
#pragma endregion

#pragma region Data Structure
	class IComponentArray
	{
	public:
		virtual ~IComponentArray() = default;
		virtual void OnEntityDestroyed(EntityID entity) = 0;
	};

	template<typename T>
	class ComponentArray : public IComponentArray
	{
	public:
		void InsertData(EntityID entity, T component)
		{
			if (m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end()) return;//don't add component more than once

			size_t newIndex = m_Size;
			m_EntityToIndexMap[entity] = newIndex;
			m_IndexToEntityMap[newIndex] = entity;
			m_ComponentArray[newIndex] = component;
			++m_Size;
		}

		void RemoveData(EntityID entity)
		{
			if (m_Size == 0) return;
			if (m_EntityToIndexMap.find(entity) == m_EntityToIndexMap.end()) return;//the component doesn't exist

			size_t removeIndex = m_EntityToIndexMap[entity];
			size_t lastIndex = m_Size - 1;
			m_ComponentArray[removeIndex] = m_ComponentArray[lastIndex];

			EntityID lastEntity = m_IndexToEntityMap[lastIndex];
			m_EntityToIndexMap[lastEntity] = removeIndex;
			m_IndexToEntityMap[removeIndex] = lastEntity;

			m_EntityToIndexMap.erase(entity);
			m_IndexToEntityMap.erase(lastIndex);

			--m_Size;
		}

		bool HasData(EntityID entity)
		{
			return m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end();
		}

		T& GetData(EntityID entity)
		{
			FS_ENGINE_ASSERT(m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end(), "Component doesn't exist");//the component doesn't exist

			return m_ComponentArray[m_EntityToIndexMap[entity]];
		}

		void OnEntityDestroyed(EntityID entity) override
		{
			RemoveData(entity);//delegate the checking to later RemoveData
		}

	private:
		std::array<T, MAX_ENTITIES> m_ComponentArray;
		std::unordered_map<EntityID, size_t> m_EntityToIndexMap;
		std::unordered_map<size_t, EntityID> m_IndexToEntityMap;

		size_t m_Size;
	};

	class System
	{
	public:
		std::set<EntityID> m_Entities;
	};

#pragma endregion

#pragma region Individual Managers
	class EntityManager
	{
	public:
		EntityManager()
		{
			for (EntityID entity = 0; entity < MAX_ENTITIES; ++entity)
			{
				m_AvailableEntities.push(entity);
			}
		}

		EntityID Create()
		{
			FS_ENGINE_ASSERT(m_UsedEntityCount < MAX_ENTITIES, "Entity exceeded limit");

			EntityID id = m_AvailableEntities.front();
			m_AvailableEntities.pop();
			++m_UsedEntityCount;

			return id;
		}

		void Destroy(EntityID entity)
		{
			FS_ENGINE_ASSERT(entity < MAX_ENTITIES && entity >= 0, "Entity out of range");

			m_Signatures[entity].reset();

			m_AvailableEntities.push(entity);
			--m_UsedEntityCount;
		}

		void SetSignature(EntityID entity, Signature signature)
		{
			FS_ENGINE_ASSERT(entity < MAX_ENTITIES && entity >= 0, "Entity out of range");
			m_Signatures[entity] = signature;
		}

		Signature GetSignature(EntityID entity)
		{
			FS_ENGINE_ASSERT(entity < MAX_ENTITIES && entity >= 0, "Entity out of range");
			return m_Signatures[entity];
		}

	private:
		std::queue<EntityID> m_AvailableEntities;

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
		Ref<T> GetSystem()
		{
			const char* typeName = typeid(T).name();
			FS_ENGINE_ASSERT(m_Systems.find(typeName) != m_Systems.end(), "System was never registered");//Dangerous function
			return std::static_pointer_cast<T>(m_Systems[typeName]);
		}

		template<typename T>
		void SetSignature(Signature signature)
		{
			const char* typeName = typeid(T).name();
			FS_ENGINE_ASSERT(m_Systems.find(typeName) != m_Systems.end(), "System was never registered");//Dangerous function

			m_Signatures.insert({ typeName, signature });
		}

		void OnEntityDestroyed(EntityID entity)
		{
			for (auto& pair : m_Systems)
			{
				auto const& system = pair.second;
				system->m_Entities.erase(entity);
			}
		}

		void OnEntitySignatureChanged(EntityID entity, Signature entitySignature) {
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
#pragma endregion

#pragma region GeneralManager

	class GeneralManager
	{
	public:
		GeneralManager() = default;

		static void AddScene(Ref<Scene> scene)
		{
			m_Instance->m_EntityManagers[scene] = std::make_shared<EntityManager>();
			m_Instance->m_ComponentManagers[scene] = std::make_shared<ComponentManager>();
			m_Instance->m_SystemManagers[scene] = std::make_shared<SystemManager>();

			//Register Components
			RegisterComponent<Tag>();
			RegisterComponent<Hierarchy>();
			RegisterComponent<CameraComponent>();
			RegisterComponent<Render2D>();
			RegisterComponent<Transform2D>();
		}
		
		static void SetActiveScene(Ref<Scene> scene)
		{
			m_Instance->m_ActiveScene = scene;
			if (m_Instance->m_EntityManagers.find(scene) == m_Instance->m_EntityManagers.end())
			{
				AddScene(scene);//This is a new scene, 
			}
		}
		
		static Ref<Scene> GetActiveScene()
		{
			FS_ENGINE_ASSERT(m_Instance->m_ActiveScene != nullptr, "There is no active scene");
			return m_Instance->m_ActiveScene;
		}

		static EntityID CreateEntity()
		{
			FS_ENGINE_ASSERT(m_Instance->m_ActiveScene != nullptr, "There is no active scene");
			return m_Instance->m_EntityManagers[m_Instance->m_ActiveScene]->Create();
		}

		static void DestroyEntity(EntityID entity)
		{
			FS_ENGINE_ASSERT(m_Instance->m_ActiveScene != nullptr, "There is no active scene");
			m_Instance->m_EntityManagers[m_Instance->m_ActiveScene]->Destroy(entity);
			m_Instance->m_ComponentManagers[m_Instance->m_ActiveScene]->OnEntityDestroyed(entity);
			m_Instance->m_SystemManagers[m_Instance->m_ActiveScene]->OnEntityDestroyed(entity);
		}

		template<typename T>
		static void RegisterComponent()
		{
			FS_ENGINE_ASSERT(m_Instance->m_ActiveScene != nullptr, "There is no active scene");
			m_Instance->m_ComponentManagers[m_Instance->m_ActiveScene]->RegisterComponent<T>();
		}

		template<typename T>
		static void AddComponent(EntityID entity, T component)
		{
			FS_ENGINE_ASSERT(m_Instance->m_ActiveScene != nullptr, "There is no active scene");
			m_Instance->m_ComponentManagers[m_Instance->m_ActiveScene]->AddComponent<T>(entity, component);

			auto signature = m_Instance->m_EntityManagers[m_Instance->m_ActiveScene]->GetSignature(entity);
			signature.set(m_Instance->m_ComponentManagers[m_Instance->m_ActiveScene]->GetComponentType<T>(), true);
			m_Instance->m_EntityManagers[m_Instance->m_ActiveScene]->SetSignature(entity, signature);

			m_Instance->m_SystemManagers[m_Instance->m_ActiveScene]->OnEntitySignatureChanged(entity, signature);
		}

		template<typename T>
		static void RemoveComponent(EntityID entity)
		{
			FS_ENGINE_ASSERT(m_Instance->m_ActiveScene != nullptr, "There is no active scene");
			m_Instance->m_ComponentManagers[m_Instance->m_ActiveScene]->RemoveComponent<T>(entity);

			auto signature = m_Instance->m_EntityManagers[m_Instance->m_ActiveScene]->GetSignature(entity);
			signature.set(m_Instance->m_ComponentManagers[m_Instance->m_ActiveScene]->GetComponentType<T>(), false);
			m_Instance->m_EntityManagers[m_Instance->m_ActiveScene]->SetSignature(entity, signature);

			m_Instance->m_SystemManagers[m_Instance->m_ActiveScene]->OnEntitySignatureChanged(entity, signature);
		}

		template<typename T>
		static bool HasComponent(EntityID entity)
		{
			FS_ENGINE_ASSERT(m_Instance->m_ActiveScene != nullptr, "There is no active scene");
			return m_Instance->m_ComponentManagers[m_Instance->m_ActiveScene]->HasComponent<T>(entity);
		}

		template<typename T>
		static T& GetComponent(EntityID entity)
		{
			FS_ENGINE_ASSERT(m_Instance->m_ActiveScene != nullptr, "There is no active scene");
			return m_Instance->m_ComponentManagers[m_Instance->m_ActiveScene]->GetComponent<T>(entity);
		}

		template<typename T>
		static ComponentType GetComponentType()
		{
			FS_ENGINE_ASSERT(m_Instance->m_ActiveScene != nullptr, "There is no active scene");
			return m_Instance->m_ComponentManagers[m_Instance->m_ActiveScene]->GetComponentType<T>();
		}

		template<typename T>
		static Ref<T> RegisterSystem()
		{
			FS_ENGINE_ASSERT(m_Instance->m_ActiveScene != nullptr, "There is no active scene");
			return m_Instance->m_SystemManagers[m_Instance->m_ActiveScene]->RegisterSystem<T>();
		}

		template<typename T>
		static Ref<T> GetSystem()
		{
			FS_ENGINE_ASSERT(m_Instance->m_ActiveScene != nullptr, "There is no active scene");
			return m_Instance->m_SystemManagers[m_Instance->m_ActiveScene]->GetSystem<T>();
		}

		template<typename T>
		static void SetSystemSignature(Signature signature)
		{
			FS_ENGINE_ASSERT(m_Instance->m_ActiveScene != nullptr, "There is no active scene");
			m_Instance->m_SystemManagers[m_Instance->m_ActiveScene]->SetSignature<T>(signature);
		}

		static void RemoveScene(Ref<Scene> scene)
		{
			m_Instance->m_EntityManagers.erase(scene);
			m_Instance->m_ComponentManagers.erase(scene);
			m_Instance->m_SystemManagers.erase(scene);
		}

		static void Reset()
		{
			m_Instance->m_EntityManagers.clear();
			m_Instance->m_ComponentManagers.clear();
			m_Instance->m_SystemManagers.clear();
		}

		static bool IsSceneValid(Ref<Scene> scene)
		{
			return m_Instance->m_EntityManagers.find(scene) != m_Instance->m_EntityManagers.end();
		}

	private:
		static Scope<GeneralManager> m_Instance;

		Ref<Scene> m_ActiveScene;

		std::unordered_map<Ref<Scene>, Ref<EntityManager>> m_EntityManagers;
		std::unordered_map<Ref<Scene>, Ref<ComponentManager>> m_ComponentManagers;
		std::unordered_map<Ref<Scene>, Ref<SystemManager>> m_SystemManagers;
	};
#pragma endregion

}