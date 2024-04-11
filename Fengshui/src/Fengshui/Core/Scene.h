#pragma once

//#include "Fengshui/ECS/Old/GameEntity.h"
//#include "Fengshui/ECS/Old/Components/Components.h"

#include "Fengshui/Events/Event.h"
#include "Fengshui/ECS/ECS.h"
#include "Fengshui/ECS/Systems.h"
#include "Fengshui/ECS/Entity.h"

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

		void OnUpdate(float dt);

		void OnEvent(Event& e);

		static Ref<Scene> Init();

		void SetViewportFocused(bool focused) { m_ViewportFocused = focused; }

		CameraComponent& GetCameraComponent() { return m_SceneManager.GetComponent<CameraComponent>(); }

		void ResizeBounds(float width, float height);
		void SetZoomLevel(float zoomLevel);

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		Entity m_SceneManager;

		float m_CameraMoveSpeed = 2.0f;

		bool m_ViewportFocused = false;

		Ref<RenderSystem2D> renderSystem2D;
		Ref<CameraSystem> cameraSystem;
	};
}