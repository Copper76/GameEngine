#include "fspch.h"
#include "Scene.h"

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

		//Register Systems
		scene->renderSystem2D = GeneralManager::RegisterSystem<RenderSystem2D>();
		scene->cameraSystem = GeneralManager::RegisterSystem<CameraSystem>();

		Signature signature;
		signature.set(GeneralManager::GetComponentType<Render2D>());
		signature.set(GeneralManager::GetComponentType<Transform2D>());
		signature.set(GeneralManager::GetComponentType<Hierarchy>());//useful for allowing entities to move with parent
		GeneralManager::SetSystemSignature<RenderSystem2D>(signature);

		signature.reset();
		signature.set(GeneralManager::GetComponentType<CameraComponent>());
		GeneralManager::SetSystemSignature<CameraSystem>(signature);

		scene->m_SceneManager = Entity("Scene Manager");

		scene->m_SceneManager.AddComponent(CameraComponent{
			true,
			1.0f,
			glm::vec3(0.0f),
			1280.0f/720.0f,
			std::make_shared<OrthographicCamera>(-1.0f * 1280.0f / 720.0f * 1.0f, 1.0f * 1280.0f / 720.0f * 1.0f, -1.0f * 1.0f, 1.0f * 1.0f) });

		return scene;
	}

	void Scene::OnUpdate(float dt)
	{
		CameraComponent* cameraComp = cameraSystem->GetPrimary();
		
		if (cameraComp)
		{
			FS_ENGINE_INFO("WILL RENDER");
			if (m_ViewportFocused)
			{
				//Input handling
				if (Input::IsKeyPressed(FS_KEY_W))
				{
					cameraComp->CameraPos.y += m_CameraMoveSpeed * dt;
				}
				if (Input::IsKeyPressed(FS_KEY_S))
				{
					cameraComp->CameraPos.y -= m_CameraMoveSpeed * dt;
				}
				if (Input::IsKeyPressed(FS_KEY_A))
				{
					cameraComp->CameraPos.x -= m_CameraMoveSpeed * dt;
				}
				if (Input::IsKeyPressed(FS_KEY_D))
				{
					cameraComp->CameraPos.x += m_CameraMoveSpeed * dt;
				}

				cameraComp->Camera->SetPosition(cameraComp->CameraPos);

				if (Input::IsKeyPressed(FS_KEY_Q))
				{
					cameraComp->Camera->SetRotation(cameraComp->Camera->GetRotation() + m_CameraMoveSpeed * dt);
				}
				if (Input::IsKeyPressed(FS_KEY_E))
				{
					cameraComp->Camera->SetRotation(cameraComp->Camera->GetRotation() - m_CameraMoveSpeed * dt);
				}
			}

			//Clear the screen
			RenderCommand::Clear();

			glm::vec2 defaultCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };

			//2D Render cycle
			Renderer2D::BeginScene(cameraComp);

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
		cameraSystem->OnMouseScrolled(m_SceneManager.GetID(), e);
		m_CameraMoveSpeed = m_SceneManager.GetComponent<CameraComponent>().ZoomLevel * 2.0f;
		return false;
	}

	bool Scene::OnWindowResize(WindowResizeEvent& e)
	{
		cameraSystem->OnWindowResize(m_SceneManager.GetID(), e);
		return false;
	}

	void Scene::ResizeBounds(float width, float height)
	{
		cameraSystem->ResizeBounds(m_SceneManager.GetID(), width, height);
	}

	void Scene::SetZoomLevel(float zoomLevel)
	{
		cameraSystem->SetZoomLevel(m_SceneManager.GetID(), zoomLevel);
	}
}