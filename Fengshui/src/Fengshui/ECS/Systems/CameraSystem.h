#pragma once
#include "Fengshui/ECS/ECS.h"
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

		void AdjustCamera(EntityID entity, glm::vec3 moveDelta, glm::quat rotateDelta);

		void CalculateView(EntityID entity);
		void RecalculateViewMatrix(EntityID entity);

		EntityID GetPrimary();

	private:
		void CalculateView(CameraComponent& cameraComp);
	};
}