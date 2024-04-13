#pragma once

#include "Fengshui/Events/Event.h"
#include "Fengshui/ECS/Entity.h"
#include "Fengshui/ECS/Systems.h"

#include "Fengshui/Events/ApplicationEvent.h"
#include "Fengshui/Events/MouseEvent.h"
#include "Fengshui/Core/Input.h"

namespace Fengshui
{
	class Scene: public std::enable_shared_from_this<Scene>
	{
	public:
		Scene() {};

		~Scene();

		void OnUpdate(float dt);

		void OnEvent(Event& e);

		static Ref<Scene> Init();

		void SetViewportFocused(bool focused) { m_ViewportFocused = focused; }

		inline CameraComponent& GetCameraComponent() { return m_SceneManager->GetComponent<CameraComponent>(); }

		inline std::vector<std::string> GetAllEntityNames() { return displaySystem->GetEntityTags(); }
		inline std::set<EntityID> GetAllEntities() { return displaySystem->GetEntityIDs(); }

		void ResizeBounds(float width, float height);
		void SetZoomLevel(float zoomLevel);

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		Scope<Entity> m_SceneManager;

		float m_CameraMoveSpeed = 2.0f;

		bool m_ViewportFocused = false;

		Ref<RenderSystem2D> renderSystem2D;
		Ref<CameraSystem> cameraSystem;
		Ref<HierarchyDisplaySystem> displaySystem;
	};
}