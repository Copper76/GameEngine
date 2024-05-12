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
			ImGui::Text("Name:"); ImGui::SameLine();
			if (ImGui::InputText("##", buffer, sizeof(buffer)))
			{
				tag.Name = std::string(buffer);
			}
			ImGui::Text("EntityID:"); ImGui::SameLine(); ImGui::Text(std::to_string(entity).c_str());
		}

		if (GeneralManager::HasComponent<Hierarchy>(entity))
		{
			if (ImGui::CollapsingHeader("Hierarchy", ImGuiTreeNodeFlags_DefaultOpen))
			{
				auto& hierarchy = GeneralManager::GetComponent<Hierarchy>(entity);
			ImGui::Text("Parent:"); ImGui::SameLine(); ImGui::Text(std::to_string(hierarchy.Parent).c_str());
				if (!hierarchy.Children.empty())
				{
					ImGui::Text("Children:");
					for (EntityID id : hierarchy.Children)
					{
						ImGui::Text(std::to_string(id).c_str());
					}
				}
			}
		}

		if (GeneralManager::HasComponent<Transform>(entity))
		{
			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
			{
				bool valueChanged = false;
				auto& transform = GeneralManager::GetComponent<Transform>(entity);

				if (ImGui::DragFloat3("Position", glm::value_ptr(transform.Position), 0.5f) || ImGui::DragFloat3("Rotation", glm::value_ptr(transform.Rotation), 0.5f)
					|| ImGui::DragFloat3("Scale", glm::value_ptr(transform.Scale), 0.5f))
				{
					valueChanged = true;
				}

				if (GeneralManager::HasComponent<CameraComponent>(entity) && valueChanged)
				{
					GeneralManager::GetActiveScene()->UpdateViewMatrix(entity);
				}
			}
		}
		else
		{
			if (GeneralManager::HasComponent<Transform2D>(entity))
			{
				if (ImGui::CollapsingHeader("Transform2D", ImGuiTreeNodeFlags_DefaultOpen))
				{
					auto& transform2D = GeneralManager::GetComponent<Transform2D>(entity);

					ImGui::DragFloat3("Position", glm::value_ptr(transform2D.Position), 0.5f);
					ImGui::DragFloat("Rotation", &transform2D.Rotation, 0.1f);
					ImGui::DragFloat2("Scale", glm::value_ptr(transform2D.Scale), 0.5f);
				}
			}
		}

		if (GeneralManager::HasComponent<Render2D>(entity))
		{
			if (ImGui::CollapsingHeader("Render2D", ImGuiTreeNodeFlags_DefaultOpen))
			{
				auto& render2D = GeneralManager::GetComponent<Render2D>(entity);
			}
		}

		if (GeneralManager::HasComponent<CameraComponent>(entity))
		{
			if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
			{
				bool valueChanged = false;
				auto& camera = GeneralManager::GetComponent<CameraComponent>(entity);

				if (ImGui::Checkbox("IsOrtho", &camera.IsOrtho) || ImGui::DragFloat("Near Plane", &camera.NearPlane, 0.1f, -1.0f, 1.0f) || ImGui::DragFloat("Far Plane", &camera.FarPlane, 0.5f, 1.0f, 100.0f))
				{
					valueChanged = true;
				}

				if (camera.IsOrtho)
				{

					if (ImGui::DragFloat("Zoom Level", &camera.ZoomLevel, 0.1f))
					{
						valueChanged = true;
					}
				}
				else
				{
					if(ImGui::DragFloat("FOV", &camera.FOV, 30.0f, 90.0f, 0.5f))
					{
						valueChanged = true;
					}
				}

				if (valueChanged)
				{
					GeneralManager::GetActiveScene()->UpdateView();
				}
			}
		}
	}
}