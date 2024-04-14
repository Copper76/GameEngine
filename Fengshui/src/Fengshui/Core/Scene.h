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

		inline Ref<Entity> GetSceneManager() { return m_SceneManager; }

		inline Ref<HierarchySystem> GetHierarchySystem() { return hierarchySystem; }

		void ResizeBounds(float width, float height);
		void SetZoomLevel(float zoomLevel);

		void UpdateView();
		void UpdateViewMatrix();

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		Ref<Entity> m_RootNode;
		Ref<Entity> m_SceneManager;

		float m_CameraMoveSpeed = 2.0f;

		bool m_ViewportFocused = false;

		//Systems
		Ref<RenderSystem2D> renderSystem2D;
		Ref<CameraSystem> cameraSystem;
		Ref<HierarchyDisplaySystem> displaySystem;
		Ref<HierarchySystem> hierarchySystem;
	};
}