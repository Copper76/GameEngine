#include "fspch.h"
#include "Scene.h"
#include "Fengshui/Renderer/Renderer.h"
#include "Fengshui/Renderer/Renderer2D.h"


namespace Fengshui
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
		m_GameEntities.clear();
	}

	Ref<Scene> Scene::Init()
	{
		//set up the clear colour for the scene
		Fengshui::RenderCommand::SetClearColour({ 0.2f, 0.2f, 0.2f, 1 });

		auto scene = std::make_shared<Scene>();
		auto cameraEntity = GameEntity::Create(scene);
		auto cameraComp = cameraEntity->AddComponent<CameraComponent>();
		scene->m_CameraComponent = cameraComp;

		return scene;
	}

	Ref<GameEntity> Scene::GetGameEntity(uint32_t id)
	{
		return m_GameEntities[id];
	}

	uint32_t Scene::RegisterEntity(Ref<GameEntity> entity)
	{
		m_GameEntities[m_NextEntityID] = entity;
		return m_NextEntityID++;
	}

	bool Scene::RegisterComponent(uint32_t entityID, Ref<Component> component)
	{
		if (m_Components[component->GetComponentType()][entityID] != nullptr)
		{
			return false;
		}
		component->SetEntityID(entityID);
		m_Components[component->GetComponentType()][entityID] = component;
		return true;
	}

	void Scene::RemoveEntity(uint32_t entityID)
	{
		for (Ref<Component> comp : GetGameEntity(entityID)->GetComponents())
		{
			RemoveComponent(entityID, comp);
		}

		m_GameEntities.erase(entityID);
	}

	void Scene::RemoveEntity(Ref<GameEntity> entity)
	{
		uint32_t entityID = entity->GetID();
		for (Ref<Component> comp : GetGameEntity(entityID)->GetComponents())
		{
			RemoveComponent(entityID, comp);
		}

		m_GameEntities.erase(entityID);
	}

	void Scene::RemoveComponent(uint32_t entityID, Ref<Component> component)
	{
		m_Components[component->GetComponentType()].erase(entityID);
	}

	void Scene::OnUpdate(float dt)
	{
		m_CameraComponent->OnUpdate(dt);
		auto transformComponents = m_Components[ComponentType::ComponentTransform];
		auto transform2DComponents = m_Components[ComponentType::ComponentTransform2D];
		auto renderComponents = m_Components[ComponentType::ComponentRender];
		auto render2DComponents = m_Components[ComponentType::ComponentRender2D];

		//Clear the screen
		RenderCommand::Clear();

		//Render cycle
		Renderer::BeginScene(shared_from_this());
		//Fengshui::Renderer::BeginScene(m_Camera);

		for (auto kv : renderComponents)
		{
			//std::dynamic_pointer_cast<RenderComponent>(kv.second)->OnUpdate(std::dynamic_pointer_cast<TransformComponent>(transformComponents[kv.first]));
		}

		Renderer::EndScene();

		//2D Render cycle
		Renderer2D::BeginScene(shared_from_this());
		for (int i = -5.0f; i < 5.0f; i+= 1.0f)
		{
			for (int j = -5.0f; j < 5.0f; j += 1.0f)
			{

				Renderer2D::DrawQuad({ i, j }, { 0.5f, 0.5f }, 0.0f, 1.0f, nullptr, { (i+0.5f)/10.0f, (j + 0.5f) / 10.0f, 1.0f, 1.0f });
			}
		}

		for (auto kv : render2DComponents)
		{
			std::dynamic_pointer_cast<RenderComponent2D>(kv.second)->OnUpdate(std::dynamic_pointer_cast<TransformComponent2D>(transform2DComponents[kv.first]));
		}

		Renderer2D::EndScene();
	}

	void Scene::OnEvent(Event& e)
	{
		m_CameraComponent->OnEvent(e);
	}
}