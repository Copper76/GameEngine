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
		if (cameraComp.IsOrtho)
		{
			CalculateView(cameraComp);
		}
	}

	bool CameraSystem::OnMouseScrolled(EntityID entity, MouseScrolledEvent& e)
	{
		auto& cameraComp = GeneralManager::GetComponent<CameraComponent>(entity);
		cameraComp.ZoomLevel -= e.GetYOffset() * 0.1f;
		cameraComp.ZoomLevel = std::max(cameraComp.ZoomLevel, 0.5f);
		if (cameraComp.IsOrtho)
		{
			CalculateView(cameraComp);
		}
		return false;
	}

	bool CameraSystem::OnWindowResize(EntityID entity, WindowResizeEvent& e)
	{
		ResizeBounds(entity, (float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}

	void CameraSystem::AdjustCamera(EntityID entity, glm::vec3 moveDelta, glm::quat rotateDelta)
	{
		auto& transform = GeneralManager::GetComponent<Transform>(entity);

		transform.Position += moveDelta;
		transform.Rotation *= rotateDelta;

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

	void CameraSystem::SetPrimary(EntityID entity)
	{
		for (EntityID cameraID : m_Entities)
		{
			auto& cameraComp = GeneralManager::GetComponent<CameraComponent>(cameraID);
			if (cameraComp.Primary)
			{
				cameraComp.Primary = false;
				break;
			}
		}
		GeneralManager::GetComponent<CameraComponent>(entity).Primary = true;
	}

	void CameraSystem::CalculateView(CameraComponent& cameraComp)
	{
		if (cameraComp.IsOrtho)
		{
			cameraComp.ProjectionMatrix = glm::ortho(-1.0f * cameraComp.AspectRatio * cameraComp.ZoomLevel, 1.0f * cameraComp.AspectRatio * cameraComp.ZoomLevel, -1.0f * cameraComp.ZoomLevel, 1.0f * cameraComp.ZoomLevel, cameraComp.OrthoNearPlane, cameraComp.OrthoFarPlane);
		}
		else
		{
			cameraComp.ProjectionMatrix = glm::perspective(glm::radians(cameraComp.FOV), cameraComp.AspectRatio, cameraComp.PersNearPlane, cameraComp.PersFarPlane);
		}
		cameraComp.ViewProjectionMatrix = cameraComp.ProjectionMatrix * cameraComp.ViewMatrix;
	}

	void CameraSystem::CalculateView(EntityID entity)
	{
		auto& cameraComp = GeneralManager::GetComponent<CameraComponent>(entity);

		CalculateView(cameraComp);

		//cameraComp.ProjectionMatrix = glm::ortho(-1.0f * cameraComp.AspectRatio * cameraComp.ZoomLevel, 1.0f * cameraComp.AspectRatio * cameraComp.ZoomLevel, -1.0f * cameraComp.ZoomLevel, 1.0f * cameraComp.ZoomLevel, -1.0f, 1.0f);
		//cameraComp.ViewProjectionMatrix = cameraComp.ProjectionMatrix * cameraComp.ViewMatrix;
	}

	void CameraSystem::RecalculateViewMatrix(EntityID entity)
	{
		auto transform = GeneralManager::GetComponent<Transform>(entity);
		auto hierarchyData = GeneralManager::GetComponent<Hierarchy>(entity);
		auto& cameraComp = GeneralManager::GetComponent<CameraComponent>(entity);

		EntityID curr = entity;

		glm::mat4 camTransform = glm::identity<glm::mat4>();

		while (curr != 0)
		{
			if (GeneralManager::HasComponent<Transform>(curr))
			{
				camTransform = GeneralManager::GetComponent<Transform>(curr).GetTransform() * camTransform;
			}

			hierarchyData = GeneralManager::GetComponent<Hierarchy>(curr);
			curr = hierarchyData.Parent;
		}
		cameraComp.ViewMatrix = glm::inverse(camTransform);

		cameraComp.ViewProjectionMatrix = cameraComp.ProjectionMatrix * cameraComp.ViewMatrix; //order based on opengl
	}
}