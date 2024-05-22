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
				/*if (ImGui::Button("Remove"))
				{
					GeneralManager::RemoveComponent<Transform>(entity);
				}
				else*/
				{
					bool valueChanged = false;
					auto& transform = GeneralManager::GetComponent<Transform>(entity);

					valueChanged |= ImGui::DragFloat3("Position", glm::value_ptr(transform.Position), 0.5f);

					glm::vec3 eulerAngles = glm::degrees(glm::eulerAngles(transform.Rotation));

					if (ImGui::DragFloat3("Rotation", glm::value_ptr(eulerAngles), 0.5f, -90.0f, 90.0f))
					{
						valueChanged = true;
						transform.Rotation = glm::quat(glm::radians(eulerAngles));
					}
					valueChanged |= ImGui::DragFloat3("Scale", glm::value_ptr(transform.Scale), 0.5f);

					if (GeneralManager::HasComponent<CameraComponent>(entity) && valueChanged)
					{
						GeneralManager::GetActiveScene()->UpdateViewMatrix(entity);
					}
				}
			}
		}
		else
		{
			if (GeneralManager::HasComponent<Transform2D>(entity))
			{
				if (ImGui::CollapsingHeader("Transform2D", ImGuiTreeNodeFlags_DefaultOpen))
				{
					/*if (ImGui::Button("Remove"))
					{
						GeneralManager::RemoveComponent<Transform2D>(entity);
					}
					else*/
					{
						auto& transform2D = GeneralManager::GetComponent<Transform2D>(entity);

						ImGui::DragFloat3("Position", glm::value_ptr(transform2D.Position), 0.5f);
						ImGui::DragFloat("Rotation", &transform2D.Rotation, 0.1f);
						ImGui::DragFloat2("Scale", glm::value_ptr(transform2D.Scale), 0.5f);
					}
				}
			}
		}

		if (GeneralManager::HasComponent<Rigidbody>(entity))
		{
			if (ImGui::CollapsingHeader("Rigidbody", ImGuiTreeNodeFlags_DefaultOpen))
			{
				/*if (ImGui::Button("Remove"))
				{
					GeneralManager::RemoveComponent<Render>(entity);
				}
				else*/
				{
					auto& rigidbody = GeneralManager::GetComponent<Rigidbody>(entity);

					ImGui::DragFloat3("Velocity", glm::value_ptr(rigidbody.LinearVelocity));
					//ImGui::DragFloat4("Angular Velocity", glm::value_ptr(rigidbody.AngularVelocity));
					glm::vec3 eulerAngularVelocity = glm::degrees(rigidbody.AngularVelocity);

					if (ImGui::DragFloat3("Angular Vel", glm::value_ptr(eulerAngularVelocity), 0.5f, -90.0f, 90.0f))
					{
						rigidbody.AngularVelocity = glm::radians(eulerAngularVelocity);
					}

					ImGui::DragFloat3("Gravity", glm::value_ptr(rigidbody.Gravity), 0.1f);

					float mass = rigidbody.InvMass == 0 ? 0.0f : 1.0f / rigidbody.InvMass;
					if (ImGui::DragFloat("Mass", &mass, 0.1f, 0.0f, std::numeric_limits<float>::max()))
					{
						rigidbody.InvMass = mass == 0.0f ? 0.0f : 1.0f / mass;
					}

					ImGui::DragFloat("Friction", &rigidbody.Friction, 0.1f, 0.0f, 1.0f);
					ImGui::DragFloat("Elasticity", &rigidbody.Elasticity, 0.1f, 0.0f, 1.0f);
				}
			}
		}

		if (GeneralManager::HasComponent<Collider>(entity))
		{
			if (ImGui::CollapsingHeader("Collider", ImGuiTreeNodeFlags_DefaultOpen))
			{
				/*if (ImGui::Button("Remove"))
				{
					GeneralManager::RemoveComponent<Render>(entity);
				}
				else*/
				{
					auto& collider = GeneralManager::GetComponent<Collider>(entity);

					ImGui::DragFloat3("Offset", glm::value_ptr(collider.Offset));
					ImGui::DragFloat3("Size", glm::value_ptr(collider.Size));
				}
			}
		}

		if (GeneralManager::HasComponent<Render>(entity))
		{
			if (ImGui::CollapsingHeader("Render", ImGuiTreeNodeFlags_DefaultOpen))
			{
				/*if (ImGui::Button("Remove"))
				{
					GeneralManager::RemoveComponent<Render>(entity);
				}
				else*/
				{
					auto& render = GeneralManager::GetComponent<Render>(entity);

					ImGui::ColorEdit4("Colour", glm::value_ptr(render.Colour));
				}
			}
		}
		else if (GeneralManager::HasComponent<Render2D>(entity))
		{
			if (ImGui::CollapsingHeader("Render2D", ImGuiTreeNodeFlags_DefaultOpen))
			{
				/*if (ImGui::Button("Remove"))
				{
					GeneralManager::RemoveComponent<Render2D>(entity);
				}
				else*/
				{
					auto& render2D = GeneralManager::GetComponent<Render2D>(entity);

					ImGui::ColorEdit4("Colour", glm::value_ptr(render2D.Colour));
				}
			}
		}

		if (GeneralManager::HasComponent<CameraComponent>(entity))
		{
			if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
			{
				/*if (ImGui::Button("Remove"))
				{
					GeneralManager::RemoveComponent<CameraComponent>(entity);
				}
				else*/
				{
					bool valueChanged = false;
					auto& camera = GeneralManager::GetComponent<CameraComponent>(entity);

					if (ImGui::Checkbox("IsOrtho", &camera.IsOrtho))
					{
						valueChanged = true;
					}

					if (camera.IsOrtho)
					{

						if (ImGui::DragFloat("Zoom Level", &camera.ZoomLevel, 0.1f) || ImGui::DragFloat("Near Plane", &camera.OrthoNearPlane, 0.1f, -1.0f, 1.0f) || ImGui::DragFloat("Far Plane", &camera.OrthoFarPlane, 0.5f, 1.0f, 100.0f))
						{
							valueChanged = true;
						}
					}
					else
					{
						if (ImGui::DragFloat("FOV", &camera.FOV, 30.0f, 90.0f, 0.5f) || ImGui::DragFloat("Near Plane", &camera.PersNearPlane, 0.1f, 0.1f, 1.0f) || ImGui::DragFloat("Far Plane", &camera.PersFarPlane, 0.5f, 1.0f, 1000.0f))
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
		/*
		if (ImGui::BeginCombo("Components", "Add Component"))
		{
			if (ImGui::Selectable("Transform"))
			{
				GeneralManager::AddComponent<Transform>(entity, Transform());
			}
			ImGui::EndCombo();
		}
		*/
	}
}