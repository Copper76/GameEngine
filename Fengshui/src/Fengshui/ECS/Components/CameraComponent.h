#pragma once

#include "Fengshui/ECS/Components/BaseComponent.h"
#include "Fengshui/Renderer/Camera.h"

namespace Fengshui
{
	class CameraComponent : public Component
	{
	public:
		CameraComponent(uint32_t id) : Component(id), m_Camera(-1.0f, 1.0f, -1.0f, 1.0f)
		{
			FS_INFO("Initiated");
		}
		
		CameraComponent(uint32_t id, float left, float right, float bottom, float top, float aspectratio = (16.0f / 9.0f)) : Component(id), m_Camera(left, right, bottom, top, aspectratio)
		{

		}

		COMPONENT_CLASS_TYPE(ComponentCamera);

		void SetPosition(const glm::vec3& position) { m_Camera.SetPosition(position); }
		void SetRotation(float rotation) { m_Camera.SetRotation(rotation); }

		const glm::vec3 GetPosition() const { return m_Camera.GetPosition(); }
		const float GetRotation() const { return m_Camera.GetRotation(); }

		const glm::mat4& GetProjectionMatrix() const { return m_Camera.GetProjectionMatrix(); }
		const glm::mat4& GetViewMatrix() const { return m_Camera.GetViewMatrix(); }
		const glm::mat4& GetViewProjectionMatrix() const { return m_Camera.GetViewProjectionMatrix(); }

	private:
		OrthographicCamera m_Camera;
	};

}