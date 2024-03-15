#pragma once

#include "Fengshui/ECS/GameEntity.h"
#include "Fengshui/ECS/Components/TransformComponent.h"
#include "Fengshui/ECS/Components/RenderComponent.h"
#include "Fengshui/ECS/Components/CameraComponent.h"

namespace Fengshui
{
	class GameEntity;

	class Scene
	{
	public:
		Scene();

		~Scene();

		uint32_t RegisterEntity(GameEntity* entity);

		bool RegisterComponent(uint32_t entityID, Ref<Component> component);

		void RemoveComponent(uint32_t entityID, Ref<Component> component);

		void OnUpdate(float dt);

		static Ref<Scene> Init();

		GameEntity* GetGameEntity(uint32_t id);

		Ref<CameraComponent> GetCameraComponent() { return m_CameraComponent; }

	private:
		uint32_t m_NextEntityID = 1;
		std::unordered_map<uint32_t, GameEntity*> m_GameEntities;
		//std::unordered_map<ComponentType,std::unordered_map<uint32_t, Ref<Component>>> m_Components;
		std::unordered_map<ComponentType,std::unordered_map<uint32_t, Ref<Component>>> m_EntityComponents;

		Ref<CameraComponent> m_CameraComponent;
	};
}