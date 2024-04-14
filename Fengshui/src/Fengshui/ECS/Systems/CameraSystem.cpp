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

	void CameraSystem::AdjustCamera(EntityID entity, glm::vec3 moveDelta, float rotateDelta)
	{
		auto& transform = GeneralManager::GetComponent<Transform2D>(entity);
		auto& cameraComp = GeneralManager::GetComponent<CameraComponent>(entity);

		transform.Position += moveDelta;
		transform.Rotation += rotateDelta;

		RecalculateViewMatrix(entity);
	}

	EntityID CameraSystem::GetPrimary()
	{
		for (EntityID cameraID : m_Entities)
		{
			auto& cameraComp = GeneralManager::GetComponent<CameraComponent>(cameraID);
			if (cameraComp.Primary)
			{
				return cameraID;
			}
		}
		return 0;
	}

	void CameraSystem::CalculateView(CameraComponent& cameraComp)
	{
		cameraComp.ProjectionMatrix = glm::ortho(-1.0f * cameraComp.AspectRatio * cameraComp.ZoomLevel, 1.0f * cameraComp.AspectRatio * cameraComp.ZoomLevel, -1.0f * cameraComp.ZoomLevel, 1.0f * cameraComp.ZoomLevel, -1.0f, 1.0f);
		cameraComp.ViewProjectionMatrix = cameraComp.ProjectionMatrix * cameraComp.ViewMatrix;
	}

	void CameraSystem::CalculateView(EntityID entity)
	{
		auto& cameraComp = GeneralManager::GetComponent<CameraComponent>(entity);

		cameraComp.ProjectionMatrix = glm::ortho(-1.0f * cameraComp.AspectRatio * cameraComp.ZoomLevel, 1.0f * cameraComp.AspectRatio * cameraComp.ZoomLevel, -1.0f * cameraComp.ZoomLevel, 1.0f * cameraComp.ZoomLevel, -1.0f, 1.0f);
		cameraComp.ViewProjectionMatrix = cameraComp.ProjectionMatrix * cameraComp.ViewMatrix;
	}

	void CameraSystem::RecalculateViewMatrix(EntityID entity)
	{
		auto& transform = GeneralManager::GetComponent<Transform2D>(entity);
		auto& cameraComp = GeneralManager::GetComponent<CameraComponent>(entity);

		glm::mat4 camTransform = glm::translate(glm::mat4(1.0f), transform.Position) * glm::rotate(glm::mat4(1.0f), glm::radians(transform.Rotation), glm::vec3(0, 0, 1));

		cameraComp.ViewMatrix = glm::inverse(camTransform);
		cameraComp.ViewProjectionMatrix = cameraComp.ProjectionMatrix * cameraComp.ViewMatrix; //order based on opengl
	}
}