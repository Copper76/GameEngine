#pragma once

#include "Fengshui/Events/Event.h"
#include "Fengshui/ECS/Entity.h"
#include "Fengshui/ECS/Systems.h"

#include "Fengshui/Events/ApplicationEvent.h"
#include "Fengshui/Events/MouseEvent.h"
#include "Fengshui/Core/Input.h"

#include "Fengshui/Physics/Physics/Constraints.h"
#include "Fengshui/Physics/Physics/Manifold.h"

namespace Fengshui
{
	class Scene
	{
	public:
		Scene() {};

		~Scene();

		void OnUpdate(float dt);
		void OnFixedUpdate(float dt);
		void OnRender();

		void OnEvent(Event& e);

		static Ref<Scene> Init();

		void SetViewportFocused(bool focused) { m_ViewportFocused = focused; }

		inline Ref<Entity> GetSceneManager() { return m_SceneManager; }

		inline Ref<HierarchySystem> GetHierarchySystem() { return m_HierarchySystem; }

		void ResizeBounds(float width, float height);
		void SetZoomLevel(float zoomLevel);

		void AdjustCamera(EntityID entity, glm::vec3 moveDelta, glm::quat rotateDelta);

		void UpdateView();
		void UpdateViewMatrix(EntityID entity);

		void AddConstraint(Ref<Constraint> constraint);
		void RemoveConstraint(Ref<Constraint> constraint);

		void SetPrimaryCamera(Ref<Entity> entity);
		void SetPrimaryCamera(EntityID entity);

		Ref<Entity> CreateEntity();

		inline void AddEntity(Ref<Entity> entity) { m_Entities[entity->GetID()] = entity; }
		inline void RemoveEntity(Ref<Entity> entity) { m_Entities.erase(entity->GetID()); }
		inline void RemoveEntity(EntityID entity) { m_Entities.erase(entity); }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		Ref<Entity> m_RootNode;
		Ref<Entity> m_SceneManager;

		std::unordered_map<EntityID, Ref<Entity>> m_Entities;

	private:
		std::vector<Ref<Constraint>> m_Constraints;
		Ref<ManifoldCollector> m_Manifolds;

	private:
		float m_CameraMoveSpeed = 0.1f;
		bool m_ViewportFocused = false;

	private:
		//Systems
		Ref<RenderSystem> m_RenderSystem;
		Ref<RenderSystem2D> m_RenderSystem2D;
		Ref<CameraSystem> m_CameraSystem;
		Ref<HierarchySystem> m_HierarchySystem;
		Ref<GravitySystem> m_GravitySystem;
		Ref<PhysicsSystem> m_PhysicsSystem;
		Ref<TransformSystem> m_TransformSystem;
	};
}