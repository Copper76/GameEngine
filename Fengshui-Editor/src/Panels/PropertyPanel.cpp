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
		size_t imGuiID = 0;
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

		bool colliderChanged = false;//Used to track if collider needs to be updated

		if (GeneralManager::HasComponent<Transform>(entity))
		{
			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
			{
				bool toRemove = false;
				if (ImGui::Button(("Remove##" + std::to_string(imGuiID++)).c_str()))
				{
					toRemove = true;
				}
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
					if (ImGui::DragFloat3("Scale", glm::value_ptr(transform.Scale), 0.5f))
					{
						valueChanged = true;
						colliderChanged = true;
					}

					if (GeneralManager::HasComponent<CameraComponent>(entity) && valueChanged)
					{
						GeneralManager::GetActiveScene()->UpdateViewMatrix(entity);
					}
				}
				if (toRemove)
				{
					GeneralManager::RemoveComponent<Transform>(entity);
					--imGuiID;
				}
			}
		}
		else
		{
			if (GeneralManager::HasComponent<Transform2D>(entity))
			{
				if (ImGui::CollapsingHeader("Transform2D", ImGuiTreeNodeFlags_DefaultOpen))
				{
					bool toRemove = false;
					if (ImGui::Button(("Remove##" + std::to_string(imGuiID++)).c_str()))
					{
						toRemove = true;
					}
					{
						auto& transform2D = GeneralManager::GetComponent<Transform2D>(entity);

						ImGui::DragFloat3("Position", glm::value_ptr(transform2D.Position), 0.5f);
						ImGui::DragFloat("Rotation", &transform2D.Rotation, 0.1f);
						ImGui::DragFloat2("Scale", glm::value_ptr(transform2D.Scale), 0.5f);
					}
					if (toRemove)
					{
						GeneralManager::RemoveComponent<Transform2D>(entity);
						--imGuiID;
					}
				}
			}
		}

		if (GeneralManager::HasComponent<Rigidbody>(entity))
		{
			if (ImGui::CollapsingHeader("Rigidbody", ImGuiTreeNodeFlags_DefaultOpen))
			{
				bool toRemove = false;
				if (ImGui::Button(("Remove##" + std::to_string(imGuiID++)).c_str()))
				{
					toRemove = true;
				}
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
						rigidbody.InvMass = (mass == 0.0f) ? 0.0f : 1.0f / mass;
					}

					ImGui::DragFloat("Friction", &rigidbody.Friction, 0.1f, 0.0f, 1.0f);
					ImGui::DragFloat("Elasticity", &rigidbody.Elasticity, 0.1f, 0.0f, 1.0f);
				}
				if (toRemove)
				{
					GeneralManager::RemoveComponent<Rigidbody>(entity);
					--imGuiID;
				}
			}
		}

		if (GeneralManager::HasComponent<Collider>(entity))
		{
			if (ImGui::CollapsingHeader("Collider", ImGuiTreeNodeFlags_DefaultOpen))
			{
				bool toRemove = false;
				if (ImGui::Button(("Remove##" + std::to_string(imGuiID++)).c_str()))
				{
					toRemove = true;
				}
				{
					auto& collider = GeneralManager::GetComponent<Collider>(entity);

					if (collider.Shape->GetType() != ShapeType::SHAPE_SPHERE)
					{
						colliderChanged |= ImGui::DragFloat3("Offset", glm::value_ptr(collider.Offset));
						colliderChanged |= ImGui::DragFloat3("Size", glm::value_ptr(collider.Size));
					}

					glm::vec3 globalScale = collider.Size * TransformSystem::GetWorldTransform(entity).Scale;

					if (ImGui::BeginCombo("Shape##1", collider.Shape->GetDisplayName().c_str()))
					{
						if (ImGui::Selectable("Box Collider"))
						{
							collider.Shape = new PhysicalShapeBox();
							collider.Shape->Rebuild(collider.Offset, globalScale);
						}

						if (ImGui::Selectable("Sphere Collider"))
						{
							collider.Shape = new PhysicalShapeSphere();
							collider.Shape->Rebuild(collider.Offset, globalScale);
						}

						if (ImGui::Selectable("Convex Collider"))
						{
							collider.Shape = new PhysicalShapeConvex(nullptr, 0);
							collider.Shape->Rebuild(collider.Offset, globalScale);
						}

						ImGui::EndCombo();
					}

					switch (collider.Shape->GetType())
					{
					case ShapeType::SHAPE_CUBE:
						break;
					case ShapeType::SHAPE_SPHERE:
						ImGui::DragFloat("Radius", &((PhysicalShapeSphere*)collider.Shape)->m_Radius, 0.1f, 0.0f, std::numeric_limits<float>::max());
						break;
					case ShapeType::SHAPE_CONVEX:
					{
						int num = ((PhysicalShapeConvex*)collider.Shape)->GetVertexNum();
						colliderChanged |= ImGui::DragInt("Num of Vertices", &num, 0.1f, 0, std::numeric_limits<int>::max());
						std::vector<glm::vec3> vertexCoords = ((PhysicalShapeConvex*)collider.Shape)->GetBaseCoords();
						if (num > 0)
						{
							ImGui::Text("Vertices:");
							for (int i = (int)vertexCoords.size(); i < num; i++)
							{
								vertexCoords.push_back(glm::vec3(0.0f));
							}

							for (int i = 0; i < num; i++)
							{
								ImGui::PushID(i);
								colliderChanged |= ImGui::DragFloat3("##colliderconvexdragfloat3", glm::value_ptr(vertexCoords[i]), 0.1f);
								ImGui::PopID();
							}
						}
						if (colliderChanged)
						{
							collider.Shape->Build(vertexCoords.data(), (int)vertexCoords.size(), collider.Offset, globalScale);
							colliderChanged = false;
						}
						break;
					}
					default:
						FS_ENGINE_ASSERT(false, "Unsupported collider type");
						break;
					}

					if (colliderChanged)
					{
						collider.Shape->Rebuild(collider.Offset, globalScale);
					}
				}
				if (toRemove)
				{
					GeneralManager::RemoveComponent<Collider>(entity);
					--imGuiID;
				}
			}
		}

		if (GeneralManager::HasComponent<Render>(entity))
		{
			if (ImGui::CollapsingHeader("Render", ImGuiTreeNodeFlags_DefaultOpen))
			{
				bool toRemove = false;
				if (ImGui::Button(("Remove##" + std::to_string(imGuiID++)).c_str()))
				{
					toRemove = true;
				}
				{
					auto& render = GeneralManager::GetComponent<Render>(entity);

					ImGui::ColorEdit4("Colour", glm::value_ptr(render.Colour));

					ImGui::DragFloat("Tiling Factor", &render.TilingFactor, 0.1f, -1.0f, std::numeric_limits<float>::max());

					if (ImGui::BeginCombo("Shape##2", render.Shape->GetDisplayName().c_str()))
					{
						if (ImGui::Selectable("Cube"))
						{
							render.Shape = new RenderShapeCube();
						}

						if (ImGui::Selectable("Sphere"))
						{
							render.Shape = new RenderShapeSphere();
						}

						if (ImGui::Selectable("Convex"))
						{
							render.Shape = new RenderShapeConvex(nullptr, 0);
						}

						ImGui::EndCombo();
					}

					switch (render.Shape->GetType())
					{
					case ShapeType::SHAPE_CUBE:
						break;
					case ShapeType::SHAPE_SPHERE:
					{
						bool changed = false;
						changed |= ImGui::DragFloat("Radius", &((RenderShapeSphere*)render.Shape)->GetRadius(), 0.1f, 0.0f, std::numeric_limits<float>::max());
						changed |= ImGui::DragInt("Divisions", &((RenderShapeSphere*)render.Shape)->GetDivisions(), 0.1f, 1, 32);

						if (changed)
						{
							((RenderShapeSphere*)render.Shape)->Rebuild();
						}
					}
						break;
					case ShapeType::SHAPE_CONVEX:
					{
						bool changed = false;
						int num = ((RenderShapeConvex*)render.Shape)->GetVertexNum();
						changed |= ImGui::DragInt("Num of Vertices", &num, 0.1f, 0, std::numeric_limits<int>::max());
						std::vector<glm::vec3> vertexCoords = ((RenderShapeConvex*)render.Shape)->GetVertexCoords();
						if (num > 0)
						{
							ImGui::Text("Vertices:");
							for (int i = (int)vertexCoords.size(); i < num; i++)
							{
								vertexCoords.push_back(glm::vec3(0.0f));
							}

							for (int i = 0; i < num; i++)
							{
								ImGui::PushID(i);
								changed |= ImGui::DragFloat3("##renderconvexdragfloat3", glm::value_ptr(vertexCoords[i]), 0.1f);
								ImGui::PopID();
							}
						}
						if (changed)
						{
							((RenderShapeConvex*)render.Shape)->Rebuild(vertexCoords);
						}
						break;
					}
					default:
						FS_ENGINE_ASSERT(false, "Unsupported render type");
					}
				}
				if (toRemove)
				{
					GeneralManager::RemoveComponent<Render>(entity);
					--imGuiID;
				}
			}
		}
		else if (GeneralManager::HasComponent<Render2D>(entity))
		{
			if (ImGui::CollapsingHeader("Render2D", ImGuiTreeNodeFlags_DefaultOpen))
			{
				bool toRemove = false;
				if (ImGui::Button(("Remove##" + std::to_string(imGuiID++)).c_str()))
				{
					toRemove = true;
				}
				{
					auto& render2D = GeneralManager::GetComponent<Render2D>(entity);

					ImGui::ColorEdit4("Colour", glm::value_ptr(render2D.Colour));
				}
				if (toRemove)
				{
					GeneralManager::RemoveComponent<Render2D>(entity);
					--imGuiID;
				}
			}
		}

		if (GeneralManager::HasComponent<CameraComponent>(entity))
		{
			if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
			{
				bool toRemove = false;
				if (ImGui::Button(("Remove##" + std::to_string(imGuiID++)).c_str()))
				{
					toRemove = true;
				}
				{
					bool valueChanged = false;
					auto& camera = GeneralManager::GetComponent<CameraComponent>(entity);

					ImGui::Checkbox("Primary", &camera.Primary);

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

				if (toRemove)
				{
					GeneralManager::RemoveComponent<CameraComponent>(entity);
					--imGuiID;
				}
			}
		}

		if (GeneralManager::HasComponent<Light>(entity))
		{
			if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
			{
				bool toRemove = false;
				if (ImGui::Button(("Remove##" + std::to_string(imGuiID++)).c_str()))
				{
					toRemove = true;
				}
				{
					auto& light = GeneralManager::GetComponent<Light>(entity);

					char* lightTypeName;
					switch (light.Type)
					{
					case LightType::GlobalLight:
						lightTypeName = "Global Light";
						break;
					case LightType::PointLight:
						lightTypeName = "Point Light";
						break;
					default:
						lightTypeName = "Unknown Light Source";
						FS_ASSERT(false, "Unknown light type");
					}

					if (ImGui::BeginCombo("Type##2", lightTypeName))
					{
						if (ImGui::Selectable("Global Light"))
						{
							light.Type = LightType::GlobalLight;
						}

						if (ImGui::Selectable("Point Light"))
						{
							light.Type = LightType::PointLight;
						}

						ImGui::EndCombo();
					}
					if (light.Type == LightType::GlobalLight)
					{
						ImGui::DragFloat3("Direction", glm::value_ptr(light.Direction), 0.1f, -1.0f, 1.0f);
						light.Direction = glm::normalize(light.Direction);
					}

					ImGui::DragFloat3("Light Colour", glm::value_ptr(light.Colour), 0.1f, 0.0f, 1.0f);
				}

				if (toRemove)
				{
					GeneralManager::RemoveComponent<CameraComponent>(entity);
					--imGuiID;
				}
			}
		}
	}
}