#include "fspch.h"
#include "Fengshui/ECS/Systems/CameraSystem.h"

namespace Fengshui
{
	void CameraSystem::ResizeBounds(EntityID entity, float width, float height)
	{
		auto& cameraComp = GeneralManager::GetComponent<CameraComponent>(entity);
		cameraComp.AspectRatio = width / height;
		CalculateView(cameraComp);
	}

	void CameraSystem::SetZoomLevel(EntityID entity, float zoomLevel)
	{
		auto& cameraComp = GeneralManager::GetComponent<CameraComponent>(entity);
		cameraComp.ZoomLevel = zoomLevel;
		CalculateView(cameraComp);
	}

	bool CameraSystem::OnMouseScrolled(EntityID entity, MouseScrolledEvent& e)
	{
		auto& cameraComp = GeneralManager::GetComponent<CameraComponent>(entity);
		cameraComp.ZoomLevel -= e.GetYOffset() * 0.1f;
		cameraComp.ZoomLevel = std::max(cameraComp.ZoomLevel, 0.5f);
		CalculateView(cameraComp);
		return false;
	}

	bool CameraSystem::OnWindowResize(EntityID entity, WindowResizeEvent& e)
	{
		ResizeBounds(entity, (float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}

	CameraComponent* CameraSystem::GetPrimary()
	{
		for (EntityID cameraID : m_Entities)
		{
			auto& cameraComp = GeneralManager::GetComponent<CameraComponent>(cameraID);
			if (cameraComp.Primary)
			{
				return &cameraComp;
			}
		}
		return nullptr;
	}

	void CameraSystem::CalculateView(CameraComponent cameraComp)
	{
		cameraComp.Camera->SetProjection(-1.0f * cameraComp.AspectRatio * cameraComp.ZoomLevel, 1.0f * cameraComp.AspectRatio * cameraComp.ZoomLevel, -1.0f * cameraComp.ZoomLevel, 1.0f * cameraComp.ZoomLevel);
	}
}