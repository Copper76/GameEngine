#include "fspch.h"
#include "Scene.h"

#include "Fengshui/Renderer/Renderer.h"
#include "Fengshui/Renderer/Renderer2D.h"


namespace Fengshui
{
	Scene::~Scene()
	{
		//m_GameEntities.clear();
	}

	Ref<Scene> Scene::Init()
	{
		//set up the clear colour for the scene
		RenderCommand::SetClearColour({ 0.2f, 0.2f, 0.2f, 1 });

		auto scene = std::make_shared<Scene>();

		//Register Components
		GeneralManager::RegisterComponent<Hierarchy>();
		GeneralManager::RegisterComponent<CameraComponent>();
		GeneralManager::RegisterComponent<Render2D>();
		GeneralManager::RegisterComponent<Transform2D>();

		//Register Systems
		scene->renderSystem2D = GeneralManager::RegisterSystem<RenderSystem2D>();

		Signature signature;
		signature.set(GeneralManager::GetComponentType<Render2D>());
		signature.set(GeneralManager::GetComponentType<Transform2D>());
		signature.set(GeneralManager::GetComponentType<Hierarchy>());//useful for allowing entities to move with parent
		GeneralManager::SetSystemSignature<RenderSystem2D>(signature);

		Entity m_SceneManager = GeneralManager::CreateEntity();

		GeneralManager::AddComponent(m_SceneManager, CameraComponent{
			1.0f, 2.0f,
			glm::vec3(0.0f),
			1280.0f/720.0f,
			std::make_shared<OrthographicCamera>(-1.0f * 1280.0f / 720.0f * 1.0f, 1.0f * 1280.0f / 720.0f * 1.0f, -1.0f * 1.0f, 1.0f * 1.0f) });

		return scene;
		/**
		auto cameraEntity = GameEntity::Create(scene);
		auto cameraComp = cameraEntity->AddComponent<CameraComponent>();
		scene->m_CameraComponent = cameraComp;

		scene->subTexture = SubTexture2D::Create(Texture2D::Create("Assets/Textures/ChernoLogo.png"), { 1, 0 }, { 512, 512 }, {1, 2});

		return scene;
		**/
	}
	/**
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
	**/

	void Scene::OnUpdate(float dt)
	{
		//Get Components
		if (m_ViewportFocused)
		{
			auto& cameraComp = GeneralManager::GetComponent<CameraComponent>(m_SceneManager);
			//Input handling
			if (Input::IsKeyPressed(FS_KEY_W))
			{
				cameraComp.m_CameraPos.y += cameraComp.CameraMoveSpeed * dt;
			}
			if (Input::IsKeyPressed(FS_KEY_S))
			{
				cameraComp.m_CameraPos.y -= cameraComp.CameraMoveSpeed * dt;
			}
			if (Input::IsKeyPressed(FS_KEY_A))
			{
				cameraComp.m_CameraPos.x -= cameraComp.CameraMoveSpeed * dt;
			}
			if (Input::IsKeyPressed(FS_KEY_D))
			{
				cameraComp.m_CameraPos.x += cameraComp.CameraMoveSpeed * dt;
			}

			cameraComp.Camera->SetPosition(cameraComp.m_CameraPos);

			if (Input::IsKeyPressed(FS_KEY_Q))
			{
				cameraComp.Camera->SetRotation(cameraComp.Camera->GetRotation() + cameraComp.CameraMoveSpeed * dt);
			}
			if (Input::IsKeyPressed(FS_KEY_E))
			{
				cameraComp.Camera->SetRotation(cameraComp.Camera->GetRotation() - cameraComp.CameraMoveSpeed * dt);
			}
		}

		//auto transformComponents = m_Components[ComponentType::ComponentTransform];
		//auto transform2DComponents = m_Components[ComponentType::ComponentTransform2D];
		//auto renderComponents = m_Components[ComponentType::ComponentRender];
		//auto render2DComponents = m_Components[ComponentType::ComponentRender2D];

		//Clear the screen
		RenderCommand::Clear();

		//Render cycle
		/**
		Renderer::BeginScene(shared_from_this());
		for (auto kv : renderComponents)
		{
			//std::dynamic_pointer_cast<RenderComponent>(kv.second)->OnUpdate(std::dynamic_pointer_cast<TransformComponent>(transformComponents[kv.first]));
		}

		Renderer::EndScene();
		**/
		//2D Render cycle
		Renderer2D::BeginScene(shared_from_this());
		for (float i = -15.0f; i < 15.0f; i+= 1.0f)
		{
			for (float j = -15.0f; j < 15.0f; j += 1.0f)
			{

				Renderer2D::DrawQuad({ i, j, -0.5f }, { 0.5f, 0.5f }, 45.0f, 1.0f, nullptr, { (i+0.5f)/10.0f, (j + 0.5f) / 10.0f, 1.0f, 1.0f });
				//Renderer2D::DrawSubQuad({ i, j }, { 0.5f, 0.5f }, 0.0f, 1.0f, subTexture, { (i+0.5f)/10.0f, (j + 0.5f) / 10.0f, 1.0f, 1.0f });
			}
		}
		//Renderer2D::DrawSubQuad({ 0, 0 }, { 0.5f, 1.0f }, 0.0f, 1.0f, subTexture, { 1.0f, 1.0f, 1.0f, 1.0f });

		/**
		for (auto kv : render2DComponents)
		{
			std::dynamic_pointer_cast<RenderComponent2D>(kv.second)->OnUpdate(std::dynamic_pointer_cast<TransformComponent2D>(transform2DComponents[kv.first]));
		}
		**/

		renderSystem2D->OnUpdate(dt);

		Renderer2D::EndScene();
	}

	void Scene::OnEvent(Event& e)
	{
		//m_CameraComponent->OnEvent(e);
		EventDispatcher eventDispatcher(e);
		eventDispatcher.Dispatch<MouseScrolledEvent>(FS_BIND_EVENT_FN(Scene::OnMouseScrolled));
		eventDispatcher.Dispatch<WindowResizeEvent>(FS_BIND_EVENT_FN(Scene::OnWindowResize));
	}

	void Scene::ResizeBounds(float width, float height)
	{
		GeneralManager::GetComponent<CameraComponent>(m_SceneManager).AspectRatio = width / height;
		CalculateView();
	}

	void Scene::SetZoomLevel(float zoomLevel)
	{
		auto& cameraComp = GeneralManager::GetComponent<CameraComponent>(m_SceneManager);
		cameraComp.ZoomLevel = zoomLevel; 
		CalculateView();
	}

	void Scene::CalculateView()
	{
		auto& cameraComp = GeneralManager::GetComponent<CameraComponent>(m_SceneManager);
		cameraComp.Camera->SetProjection(-1.0f * cameraComp.AspectRatio * cameraComp.ZoomLevel, 1.0f * cameraComp.AspectRatio * cameraComp.ZoomLevel, -1.0f * cameraComp.ZoomLevel, 1.0f * cameraComp.ZoomLevel);
	}

	bool Scene::OnMouseScrolled(MouseScrolledEvent& e)
	{
		auto& cameraComp = GeneralManager::GetComponent<CameraComponent>(m_SceneManager);
		cameraComp.ZoomLevel -= e.GetYOffset() * 0.1f;
		cameraComp.ZoomLevel = std::max(cameraComp.ZoomLevel, 0.5f);
		cameraComp.CameraMoveSpeed = cameraComp.ZoomLevel * 2.0f;
		CalculateView();
		return false;
	}

	bool Scene::OnWindowResize(WindowResizeEvent& e)
	{
		ResizeBounds((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}
}