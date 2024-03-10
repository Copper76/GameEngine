#pragma once

#include "Fengshui/ECS/GameEntity.h"
#include "Fengshui/ECS/Components/TransformComponent.h"
#include "Fengshui/ECS/Components/RenderComponent.h"
#include "Fengshui/ECS/Components/CameraComponent.h"

namespace Fengshui
{
	class Scene
	{
	public:
		Scene() : m_CameraComponent(0)
		{
			Init();
		}

		void Init();

		uint32_t CreateEntity();

		void OnUpdate(float dt);

		CameraComponent GetCameraComponent() { return m_CameraComponent; }

	private:
		uint32_t m_NextEntityID = 1;
		std::vector<GameEntity*> m_GameEntities;
		std::vector<TransformComponent*> m_TransformComponents;
		std::vector<RenderComponent*> m_RenderComponents;
		CameraComponent m_CameraComponent;
	};
}