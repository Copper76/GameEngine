#pragma once

//#include "Fengshui/ECS/Old/GameEntity.h"
//#include "Fengshui/ECS/Old/Components/Components.h"

#include "Fengshui/Events/Event.h"
#include "Fengshui/ECS/ECS.h"
#include "Fengshui/ECS/Systems.h"

#include "Fengshui/Events/ApplicationEvent.h"
#include "Fengshui/Events/MouseEvent.h"
#include "Fengshui/Core/Input.h"

namespace Fengshui
{
	class GameEntity;

	class Scene: public std::enable_shared_from_this<Scene>
	{
	public:
		Scene() = default;

		~Scene();

		//uint32_t RegisterEntity(Ref<GameEntity> entity);

		//bool RegisterComponent(uint32_t entityID, Ref<Component> component);

		//void RemoveEntity(uint32_t entityID);

		//void RemoveEntity(Ref<GameEntity> entity);

		//void RemoveComponent(uint32_t entityID, Ref<Component> component);

		void OnUpdate(float dt);

		void OnEvent(Event& e);

		static Ref<Scene> Init();

		void SetViewportFocused(bool focused) { m_ViewportFocused = focused; }

		//Ref<GameEntity> GetGameEntity(uint32_t id);

		//Ref<CameraComponent> GetCameraComponent() { return m_CameraComponent; }
		CameraComponent GetCameraComponent() { return GeneralManager::GetComponent<CameraComponent>(m_SceneManager); }

		void ResizeBounds(float width, float height);
		void SetZoomLevel(float zoomLevel);

	private:
		void CalculateView();
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		//uint32_t m_NextEntityID = 0;
		//std::unordered_map<uint32_t, Ref<GameEntity>> m_GameEntities;
		//std::unordered_map<ComponentType,std::unordered_map<uint32_t, Ref<Component>>> m_Components;

		Entity m_SceneManager;

		bool m_ViewportFocused = false;

		Ref<RenderSystem2D> renderSystem2D;
	};
}