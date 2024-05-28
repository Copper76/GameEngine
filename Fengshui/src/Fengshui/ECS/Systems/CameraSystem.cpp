#include "fspch.h"
#include "Fengshui/ECS/Systems/CameraSystem.h"
#include "Fengshui/ECS/GeneralManager.h"

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
		if (cameraComp.IsOrtho)
		{
			cameraComp.ZoomLevel = zoomLevel;
			CalculateView(cameraComp);
		}
	}

	bool CameraSystem::OnMouseScrolled(EntityID entity, MouseScrolledEvent& e)
	{
		auto& cameraComp = GeneralManager::GetComponent<CameraComponent>(entity);
		if (cameraComp.IsOrtho)
		{
			cameraComp.ZoomLevel -= e.GetYOffset() * 0.1f;
			cameraComp.ZoomLevel = std::max(cameraComp.ZoomLevel, 0.5f);
			CalculateView(cameraComp);
		}
		return false;
	}

	bool CameraSystem::OnWindowResize(EntityID entity, WindowResizeEvent& e)
	{
		ResizeBounds(entity, (float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}

	void CameraSystem::AdjustCamera(EntityID entity, glm::vec3 moveDelta, glm::quat rotateDelta, Ref<TransformSystem> transformSystem)
	{
		auto& transform = GeneralManager::GetComponent<Transform>(entity);

		const auto cameraComp = GeneralManager::GetComponent<CameraComponent>(entity);

		transform.Position += transform.Rotation * (moveDelta * (cameraComp.IsOrtho ? cameraComp.ZoomLevel * 0.01f : 0.1f));
		transform.Rotation *= rotateDelta;

		RecalculateViewMatrix(entity, transformSystem);
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
	}

	void CameraSystem::RecalculateViewMatrix(EntityID entity, Ref<TransformSystem> transformSystem)
	{
		auto& cameraComp = GeneralManager::GetComponent<CameraComponent>(entity);

		glm::mat4 camTransform = transformSystem->GetWorldTransformMatrix(entity);

		cameraComp.ViewMatrix = glm::inverse(camTransform);

		cameraComp.ViewProjectionMatrix = cameraComp.ProjectionMatrix * cameraComp.ViewMatrix; //order based on opengl
	}
}