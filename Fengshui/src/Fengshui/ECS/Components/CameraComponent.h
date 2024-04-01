#pragma once

#include "Fengshui/ECS/Components/BaseComponent.h"
#include "Fengshui/Renderer/Camera.h"

#include "Fengshui/Events/ApplicationEvent.h"
#include "Fengshui/Events/MouseEvent.h"
#include "Fengshui/Core/Input.h"

namespace Fengshui
{
	class CameraComponent : public Component
	{
	public:
		CameraComponent(float aspectRatio = (1280.0f / 720.0f));

		virtual ~CameraComponent() = default;

		void OnUpdate(float dt) override;

		void OnEvent(Event& e) override;

		COMPONENT_CLASS_TYPE(ComponentCamera);

		void ResizeBounds(float width, float height);

		inline void SetZoomLevel(float zoomLevel) { m_ZoomLevel = zoomLevel; CalculateView(); };

		inline void SetPosition(const glm::vec3& position) { m_Camera->SetPosition(position); }
		inline void SetRotation(float rotation) { m_Camera->SetRotation(rotation); }

		inline const glm::vec3 GetPosition() const { return m_Camera->GetPosition(); }
		inline const float GetRotation() const { return m_Camera->GetRotation(); }

		inline const glm::mat4& GetProjectionMatrix() const { return m_Camera->GetProjectionMatrix(); }
		inline const glm::mat4& GetViewMatrix() const { return m_Camera->GetViewMatrix(); }
		inline const glm::mat4& GetViewProjectionMatrix() const { return m_Camera->GetViewProjectionMatrix(); }

	private:
		void CalculateView();
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		float m_ZoomLevel = 1.0f;
		float m_CameraMoveSpeed;
		glm::vec3 m_CameraPos;

		float m_AspectRatio;
		Ref<OrthographicCamera> m_Camera;
	};

}