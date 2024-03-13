#pragma once

#include "Fengshui/ECS/Components/BaseComponent.h"
#include "Fengshui/Renderer/Camera.h"

namespace Fengshui
{
	class CameraComponent : public Component
	{
	public:
		CameraComponent();

		virtual ~CameraComponent() = default;

		COMPONENT_CLASS_TYPE(ComponentCamera);

		inline void SetPosition(const glm::vec3& position) { m_Camera->SetPosition(position); }
		inline void SetRotation(float rotation) { m_Camera->SetRotation(rotation); }

		inline const glm::vec3 GetPosition() const { return m_Camera->GetPosition(); }
		inline const float GetRotation() const { return m_Camera->GetRotation(); }

		inline const glm::mat4& GetProjectionMatrix() const { return m_Camera->GetProjectionMatrix(); }
		inline const glm::mat4& GetViewMatrix() const { return m_Camera->GetViewMatrix(); }
		inline const glm::mat4& GetViewProjectionMatrix() const { return m_Camera->GetViewProjectionMatrix(); }

	private:
		Ref<OrthographicCamera> m_Camera;
	};

}