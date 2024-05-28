#pragma once
#include "Fengshui/ECS/Components.h"
#include "Fengshui/ECS/ECS.h"
#include "Fengshui/ECS/EntityManager.h"
#include "Fengshui/ECS/ComponentManager.h"
#include "Fengshui/ECS/SystemManager.h"

namespace Fengshui
{
	class Scene;

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
			RegisterComponent<Render>();
			RegisterComponent<Render2D>();
			RegisterComponent<Rigidbody>();
			RegisterComponent<Collider>();
			RegisterComponent<Transform>();
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
}