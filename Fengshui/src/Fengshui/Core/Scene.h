#pragma once

#include "Fengshui/ECS/GameEntity.h"
#include "Fengshui/ECS/Components/TransformComponent.h"
#include "Fengshui/ECS/Components/RenderComponent.h"
#include "Fengshui/ECS/Components/CameraComponent.h"

namespace Fengshui
{
	class GameEntity;

	class Scene: public std::enable_shared_from_this<Scene>
	{
	public:
		Scene();

		~Scene();

		uint32_t RegisterEntity(Ref<GameEntity> entity);

		bool RegisterComponent(uint32_t entityID, Ref<Component> component);

		void RemoveEntity(uint32_t entityID);

		void RemoveEntity(Ref<GameEntity> entity);

		void RemoveComponent(uint32_t entityID, Ref<Component> component);

		void OnUpdate(float dt);

		void OnEvent(Event& e);

		static Ref<Scene> Init();

		Ref<GameEntity> GetGameEntity(uint32_t id);

		Ref<CameraComponent> GetCameraComponent() { return m_CameraComponent; }

	private:
		uint32_t m_NextEntityID = 0;
		std::unordered_map<uint32_t, Ref<GameEntity>> m_GameEntities;
		std::unordered_map<ComponentType,std::unordered_map<uint32_t, Ref<Component>>> m_Components;

		Ref<CameraComponent> m_CameraComponent;
	};
}