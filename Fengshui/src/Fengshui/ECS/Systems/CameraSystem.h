#pragma once
#include "Fengshui/ECS/ECS.h"
#include "Fengshui/ECS/Systems/TransformSystem.h"

#include "Fengshui/Events/ApplicationEvent.h"
#include "Fengshui/Events/MouseEvent.h"

namespace Fengshui
{
	class CameraSystem : public System
	{
	public:
		void ResizeBounds(EntityID entity, float width, float height);
		void SetZoomLevel(EntityID entity, float zoomLevel);
		bool OnMouseScrolled(EntityID entity, MouseScrolledEvent& e);
		bool OnWindowResize(EntityID entity, WindowResizeEvent& e);

		void AdjustCamera(EntityID entity, glm::vec3 moveDelta, glm::quat rotateDelta, Ref<TransformSystem> transformSystem);

		void CalculateView(EntityID entity);
		void RecalculateViewMatrix(EntityID entity, Ref<TransformSystem> transformSystem);

		EntityID GetPrimary();
		void SetPrimary(EntityID entity);

	private:
		void CalculateView(CameraComponent& cameraComp);
	};
}