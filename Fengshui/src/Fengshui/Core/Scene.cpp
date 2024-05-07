#include "fspch.h"
#include "Scene.h"

#include "Fengshui/Renderer/Renderer2D.h"
#include "Fengshui/ECS/ECS.h"


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

		//Create Scene
		auto scene = std::make_shared<Scene>();

		GeneralManager::SetActiveScene(scene);

		//Register Systems
		scene->renderSystem2D = GeneralManager::RegisterSystem<RenderSystem2D>();
		scene->cameraSystem = GeneralManager::RegisterSystem<CameraSystem>();
		scene->displaySystem = GeneralManager::RegisterSystem<HierarchyDisplaySystem>();
		scene->hierarchySystem = GeneralManager::RegisterSystem<HierarchySystem>();

		Signature signature;
		signature.set(GeneralManager::GetComponentType<Render2D>());
		signature.set(GeneralManager::GetComponentType<Transform2D>());
		signature.set(GeneralManager::GetComponentType<Hierarchy>());//useful for allowing entities to move with parent
		GeneralManager::SetSystemSignature<RenderSystem2D>(signature);

		signature.reset();
		signature.set(GeneralManager::GetComponentType<Transform2D>());
		signature.set(GeneralManager::GetComponentType<CameraComponent>());
		GeneralManager::SetSystemSignature<CameraSystem>(signature);

		signature.reset();
		signature.set(GeneralManager::GetComponentType<Hierarchy>());
		GeneralManager::SetSystemSignature<HierarchyDisplaySystem>(signature);

		signature.reset();
		signature.set(GeneralManager::GetComponentType<Tag>());
		GeneralManager::SetSystemSignature<HierarchyDisplaySystem>(signature);



		//Property setup
		scene->m_RootNode = std::make_shared<Entity>("Root Node");

		scene->m_SceneManager = std::make_shared<Entity>("Scene Manager");

		scene->m_SceneManager->AddComponent(CameraComponent{
			true,
			1.0f,
			1280.0f / 720.0f,
			-1.0f * 1280.0f / 720.0f * 1.0f,
			1.0f * 1280.0f / 720.0f * 1.0f,
			-1.0f * 1.0f,
			1.0f * 1.0f });

		scene->m_SceneManager->AddComponent<Transform2D>();

		return scene;
	}

	void Scene::OnUpdate(float dt)
	{
		EntityID cameraComp = cameraSystem->GetPrimary();
		
		if (cameraComp != 0)
		{
			if (m_ViewportFocused)
			{
				glm::vec3 moveDelta = glm::vec3(0.0f);
				float rotateDelta = 0.0f;

				/**
				//Camera Moving
				if (Input::IsKeyPressed(FS_KEY_W))
				{
					moveDelta.y += m_CameraMoveSpeed * dt;
				}
				if (Input::IsKeyPressed(FS_KEY_S))
				{
					moveDelta.y -= m_CameraMoveSpeed * dt;
				}
				if (Input::IsKeyPressed(FS_KEY_A))
				{
					moveDelta.x -= m_CameraMoveSpeed * dt;
				}
				if (Input::IsKeyPressed(FS_KEY_D))
				{
					moveDelta.x += m_CameraMoveSpeed * dt;
				}

				if (Input::IsKeyPressed(FS_KEY_Q))
				{
					rotateDelta += m_CameraMoveSpeed * dt;
				}
				if (Input::IsKeyPressed(FS_KEY_E))
				{
					rotateDelta -= m_CameraMoveSpeed * dt;
				}
				cameraSystem->AdjustCamera(cameraComp, moveDelta, rotateDelta);
				**/
			}

			//Clear the screen
			RenderCommand::Clear();

			//glm::vec2 defaultCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };

			//2D Render cycle
			Renderer2D::BeginScene(&GeneralManager::GetComponent<CameraComponent>(cameraComp));

			renderSystem2D->OnUpdate();

			Renderer2D::EndScene();
		}
	}

	void Scene::OnEvent(Event& e)
	{
		//m_CameraComponent->OnEvent(e);
		EventDispatcher eventDispatcher(e);
		eventDispatcher.Dispatch<MouseScrolledEvent>(FS_BIND_EVENT_FN(Scene::OnMouseScrolled));
		eventDispatcher.Dispatch<WindowResizeEvent>(FS_BIND_EVENT_FN(Scene::OnWindowResize));
	}

	bool Scene::OnMouseScrolled(MouseScrolledEvent& e)
	{
		cameraSystem->OnMouseScrolled(m_SceneManager->GetID(), e);
		m_CameraMoveSpeed = m_SceneManager->GetComponent<CameraComponent>().ZoomLevel * 2.0f;
		return false;
	}

	bool Scene::OnWindowResize(WindowResizeEvent& e)
	{
		cameraSystem->OnWindowResize(m_SceneManager->GetID(), e);
		return false;
	}
	
	void Scene::ResizeBounds(float width, float height)
	{
		cameraSystem->ResizeBounds(m_SceneManager->GetID(), width, height);
	}

	void Scene::SetZoomLevel(float zoomLevel)
	{
		cameraSystem->SetZoomLevel(m_SceneManager->GetID(), zoomLevel);
	}
	void Scene::UpdateView()
	{
		cameraSystem->CalculateView(cameraSystem->GetPrimary());
	}
	void Scene::UpdateViewMatrix()
	{
		cameraSystem->RecalculateViewMatrix(cameraSystem->GetPrimary());
	}
}