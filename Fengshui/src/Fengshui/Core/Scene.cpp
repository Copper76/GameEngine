#include "fspch.h"
#include "Scene.h"
#include "Fengshui/Renderer/Renderer.h"


namespace Fengshui
{
	Scene::Scene()
	{
		m_CameraComponent = std::make_shared<CameraComponent>();
	}

	Scene::~Scene()
	{
		m_GameEntities.clear();
	}

	Ref<Scene> Scene::Init()
	{
		return std::make_shared<Scene>();
	}

	GameEntity* Scene::GetGameEntity(uint32_t id)
	{
		return m_GameEntities[id];
	}

	uint32_t Scene::RegisterEntity(GameEntity* entity)
	{
		m_GameEntities[m_NextEntityID] = entity;
		return m_NextEntityID++;
	}

	bool Scene::RegisterComponent(uint32_t entityID, Ref<Component> component)
	{
		if (m_EntityComponents[component->GetComponentType()][entityID] != nullptr)
		{
			return false;
		}
		component->SetEntityID(entityID);
		m_EntityComponents[component->GetComponentType()][entityID] = component;
		return true;
	}

	void Scene::RemoveComponent(uint32_t entityID, Ref<Component> component)
	{
		m_EntityComponents[component->GetComponentType()].erase(entityID);
	}

	void Scene::OnUpdate(float dt)
	{
		auto transformComponents = m_EntityComponents[ComponentType::ComponentTransform];
		auto renderComponents = m_EntityComponents[ComponentType::ComponentRender];

		//Render cycle
		Fengshui::Renderer::BeginScene(*GetCameraComponent());
		//Fengshui::Renderer::BeginScene(m_Camera);

		//m_TexShader->Bind();
		for (auto kv : renderComponents)
		{
			std::dynamic_pointer_cast<RenderComponent2D>(kv.second)->OnUpdate(std::dynamic_pointer_cast<TransformComponent>(transformComponents[kv.first]));
		}

		Fengshui::Renderer::EndScene();
	}
}