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

		uint32_t RegisterEntity(GameEntity* entity);

		bool RegisterComponent(Ref<Component> component, uint32_t entityID);

		void RemoveComponent(Ref<Component> component, uint32_t entityID);

		void OnUpdate(float dt);

		static Ref<Scene> Init();

		Ref<CameraComponent> GetCameraComponent() { return m_CameraComponent; }

	private:
		uint32_t m_NextEntityID = 1;
		std::unordered_map<uint32_t, GameEntity*> m_GameEntities;
		std::unordered_map<ComponentType,std::unordered_map<uint32_t, Ref<Component>>> m_Components;

		Ref<CameraComponent> m_CameraComponent;
	};
}