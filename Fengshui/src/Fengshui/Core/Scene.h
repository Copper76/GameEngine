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
		void OnRender();

		void OnEvent(Event& e);

		static Ref<Scene> Init();

		void SetViewportFocused(bool focused) { m_ViewportFocused = focused; }

		inline Ref<Entity> GetSceneManager() { return m_SceneManager; }

		inline Ref<HierarchySystem> GetHierarchySystem() { return m_HierarchySystem; }

		void ResizeBounds(float width, float height);
		void SetZoomLevel(float zoomLevel);

		void UpdateView();
		void UpdateViewMatrix(EntityID entity);

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		Ref<Entity> m_RootNode;
		Ref<Entity> m_SceneManager;

		float m_CameraMoveSpeed = 2.0f;

		bool m_ViewportFocused = false;

		//Systems
		Ref<RenderSystem> m_RenderSystem;
		Ref<RenderSystem2D> m_RenderSystem2D;
		Ref<CameraSystem> m_CameraSystem;
		Ref<HierarchySystem> m_HierarchySystem;
		Ref<PhysicsSystem> m_PhysicsSystem;
	};
}