#include "PropertyPanel.h"
#include "imgui.h"

#include <glm/gtc/type_ptr.hpp>

namespace Fengshui
{
	void Fengshui::PropertyPanel::OnImGuiRender(EntityID entity)
	{
		DrawComponents(entity);
	}

	void Fengshui::PropertyPanel::DrawComponents(EntityID entity)
	{
		if (GeneralManager::HasComponent<Tag>(entity))
		{
			auto& tag = GeneralManager::GetComponent<Tag>(entity);

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.Name.c_str());
			if (ImGui::InputText("Name", buffer, sizeof(buffer)))
			{
				tag.Name = std::string(buffer);
			}
		}

		if (GeneralManager::HasComponent<Transform2D>(entity))
		{
			if (ImGui::TreeNodeEx((void*) typeid(Transform2D).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transform = GeneralManager::GetComponent<Transform2D>(entity);

				ImGui::DragFloat3("Position", glm::value_ptr(transform.Position), 0.5f);
				ImGui::DragFloat("Rotation", &transform.Rotation, 0.1f);
				ImGui::DragFloat2("Scale", glm::value_ptr(transform.Scale), 0.5f);

				if (GeneralManager::HasComponent<CameraComponent>(entity))
				{
					GeneralManager::GetActiveScene()->UpdateViewMatrix();
				}

				ImGui::TreePop();
			}
		}

		if (GeneralManager::HasComponent<CameraComponent>(entity))
		{
			if (ImGui::TreeNodeEx((void*) typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				auto& camera = GeneralManager::GetComponent<CameraComponent>(entity);
				float originalZoom = camera.ZoomLevel;

				ImGui::DragFloat("Zoom Level", &camera.ZoomLevel, 0.1f);

				if (originalZoom != camera.ZoomLevel) 
				{
					GeneralManager::GetActiveScene()->UpdateView();
				}

				ImGui::TreePop();
			}
		}
	}
}