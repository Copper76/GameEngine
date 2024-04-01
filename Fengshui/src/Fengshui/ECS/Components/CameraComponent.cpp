#include "fspch.h"
#include "CameraComponent.h"

namespace Fengshui
{
	CameraComponent::CameraComponent(float aspectRatio)
		: m_CameraPos(0.0f), m_CameraMoveSpeed(m_ZoomLevel * 2.0f), m_AspectRatio(aspectRatio), m_Camera(std::make_shared<OrthographicCamera>(-1.0f * aspectRatio * m_ZoomLevel, 1.0f * aspectRatio * m_ZoomLevel, -1.0f * m_ZoomLevel, 1.0f * m_ZoomLevel))
	{
	}

	void CameraComponent::OnUpdate(float dt)
	{
		//Input handling
		if (Input::IsKeyPressed(FS_KEY_W))
		{
			m_CameraPos.y += m_CameraMoveSpeed * dt;
		}
		if (Input::IsKeyPressed(FS_KEY_S))
		{
			m_CameraPos.y -= m_CameraMoveSpeed * dt;
		}
		if (Input::IsKeyPressed(FS_KEY_A))
		{
			m_CameraPos.x -= m_CameraMoveSpeed * dt;
		}
		if (Input::IsKeyPressed(FS_KEY_D))
		{
			m_CameraPos.x += m_CameraMoveSpeed * dt;
		}

		SetPosition(m_CameraPos);

		if (Input::IsKeyPressed(FS_KEY_Q))
		{
			SetRotation(GetRotation() + m_CameraMoveSpeed * dt);
		}
		if (Input::IsKeyPressed(FS_KEY_E))
		{
			SetRotation(GetRotation() - m_CameraMoveSpeed * dt);
		}
	}

	void CameraComponent::OnEvent(Event& e)
	{
		EventDispatcher eventDispatcher(e);
		eventDispatcher.Dispatch<MouseScrolledEvent>(FS_BIND_EVENT_FN(CameraComponent::OnMouseScrolled));
		eventDispatcher.Dispatch<WindowResizeEvent>(FS_BIND_EVENT_FN(CameraComponent::OnWindowResize));
	}

	void CameraComponent::ResizeBounds(float width, float height)
	{
		m_AspectRatio = width / height;
		CalculateView();
	}

	void CameraComponent::CalculateView()
	{
		m_Camera->SetProjection(-1.0f * m_AspectRatio * m_ZoomLevel, 1.0f * m_AspectRatio * m_ZoomLevel, -1.0f * m_ZoomLevel, 1.0f * m_ZoomLevel);
	}

	bool CameraComponent::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset()* 0.1f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.5f);
		m_CameraMoveSpeed = m_ZoomLevel * 2.0f;
		CalculateView();
		return false;
	}

	bool CameraComponent::OnWindowResize(WindowResizeEvent& e)
	{
		ResizeBounds((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}
}