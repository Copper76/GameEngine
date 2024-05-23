#include "fspch.h"
#include "Scene.h"

#include "Fengshui/Renderer/Renderer.h"
#include "Fengshui/Renderer/Renderer2D.h"
#include "Fengshui/ECS/ECS.h"


namespace Fengshui
{
	Scene::~Scene()
	{
		//GeneralManager::RemoveScene(shared_from_this());
	}

	Ref<Scene> Scene::Init()
	{
		//set up the clear colour for the scene
		RenderCommand::SetClearColour({ 0.2f, 0.2f, 0.2f, 1 });
		RenderCommand::SetClearDepth(1.0f);

		//Create Scene
		auto scene = std::make_shared<Scene>();

		GeneralManager::SetActiveScene(scene);

		//Register Systems
		scene->m_RenderSystem = GeneralManager::RegisterSystem<RenderSystem>();
		scene->m_RenderSystem2D = GeneralManager::RegisterSystem<RenderSystem2D>();
		scene->m_CameraSystem = GeneralManager::RegisterSystem<CameraSystem>();
		scene->m_HierarchySystem = GeneralManager::RegisterSystem<HierarchySystem>();
		scene->m_GravitySystem = GeneralManager::RegisterSystem<GravitySystem>();
		scene->m_PhysicsSystem = GeneralManager::RegisterSystem<PhysicsSystem>();
		scene->m_TransformSystem = GeneralManager::RegisterSystem<TransformSystem>();

		Signature signature;
		signature.set(GeneralManager::GetComponentType<Render>());
		signature.set(GeneralManager::GetComponentType<Transform>());
		signature.set(GeneralManager::GetComponentType<Hierarchy>());
		GeneralManager::SetSystemSignature<RenderSystem>(signature);

		signature.reset();
		signature.set(GeneralManager::GetComponentType<Render2D>());
		signature.set(GeneralManager::GetComponentType<Transform2D>());
		signature.set(GeneralManager::GetComponentType<Hierarchy>());//useful for allowing entities to move with parent
		GeneralManager::SetSystemSignature<RenderSystem2D>(signature);

		signature.reset();
		signature.set(GeneralManager::GetComponentType<Transform>());
		signature.set(GeneralManager::GetComponentType<Hierarchy>());
		signature.set(GeneralManager::GetComponentType<CameraComponent>());
		GeneralManager::SetSystemSignature<CameraSystem>(signature);

		signature.reset();
		signature.set(GeneralManager::GetComponentType<Hierarchy>());
		GeneralManager::SetSystemSignature<HierarchySystem>(signature);

		signature.reset();
		signature.set(GeneralManager::GetComponentType<Rigidbody>());
		GeneralManager::SetSystemSignature<GravitySystem>(signature);

		signature.reset();
		signature.set(GeneralManager::GetComponentType<Rigidbody>());
		signature.set(GeneralManager::GetComponentType<Transform>());
		signature.set(GeneralManager::GetComponentType<Hierarchy>());
		signature.set(GeneralManager::GetComponentType<Collider>());
		GeneralManager::SetSystemSignature<PhysicsSystem>(signature);

		signature.reset();
		signature.set(GeneralManager::GetComponentType<Transform>());
		signature.set(GeneralManager::GetComponentType<Hierarchy>());
		GeneralManager::SetSystemSignature<TransformSystem>(signature);

		//Physics System
		scene->m_Manifolds = std::make_shared<ManifoldCollector>();

		//Property setup
		scene->m_RootNode = std::make_shared<Entity>("Root Node");

		scene->m_SceneManager = std::make_shared<Entity>("Scene Manager");

		/*scene->m_SceneManager->AddComponent(CameraComponent{
			true,
			1.0f,
			1280.0f / 720.0f });*/
		scene->m_SceneManager->AddComponent(CameraComponent{
			false,
			true});

		scene->m_SceneManager->AddComponent<Transform>(Transform(glm::vec3(0.0f, 0.0f, 10.0f)));

		scene->UpdateViewMatrix(scene->m_CameraSystem->GetPrimary());

		return scene;
	}

	void Scene::OnUpdate(float dt)
	{
		EntityID cameraComp = m_CameraSystem->GetPrimary();
		
		if (cameraComp != 0)
		{
			if (m_ViewportFocused)
			{
				glm::vec3 moveDelta = glm::vec3(0.0f);
				float rotateDelta = 0.0f;

				//Input handling
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
				m_CameraSystem->AdjustCamera(cameraComp, moveDelta, glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(rotateDelta))));
			}
		}
	}

	void Scene::OnFixedUpdate(float dt)
	{
		float dt_sec = dt * 0.5f;
		for (int i = 0; i < 2; i++)
		{
			m_GravitySystem->OnUpdate(dt_sec);
			m_PhysicsSystem->OnUpdate(dt_sec, m_Manifolds, m_Constraints);
		}
	}

	void Scene::OnRender()
	{
		EntityID cameraComp = m_CameraSystem->GetPrimary();

		if (cameraComp != 0)
		{
			//Clear the screen
			RenderCommand::Clear();

			//3D Render cycle
			Renderer::BeginScene(&GeneralManager::GetComponent<CameraComponent>(cameraComp));

			m_RenderSystem->OnRender(m_TransformSystem);

			Renderer::EndScene();

			//2D Render cycle
			Renderer2D::BeginScene(&GeneralManager::GetComponent<CameraComponent>(cameraComp));

			m_RenderSystem2D->OnRender();

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
		m_CameraSystem->OnMouseScrolled(m_SceneManager->GetID(), e);
		m_CameraMoveSpeed = m_SceneManager->GetComponent<CameraComponent>().ZoomLevel * 2.0f;
		return false;
	}

	bool Scene::OnWindowResize(WindowResizeEvent& e)
	{
		m_CameraSystem->OnWindowResize(m_SceneManager->GetID(), e);
		return false;
	}
	
	void Scene::ResizeBounds(float width, float height)
	{
		m_CameraSystem->ResizeBounds(m_SceneManager->GetID(), width, height);
	}

	void Scene::SetZoomLevel(float zoomLevel)
	{
		m_CameraSystem->SetZoomLevel(m_SceneManager->GetID(), zoomLevel);
	}

	void Scene::UpdateView()
	{
		m_CameraSystem->CalculateView(m_CameraSystem->GetPrimary());
	}

	void Scene::UpdateViewMatrix(EntityID entity)
	{
		m_CameraSystem->RecalculateViewMatrix(entity);
	}

	void Scene::AddConstraint(Constraint* constraint)
	{
		m_Constraints.push_back(constraint);
	}

	void Scene::RemoveConstraint(Constraint* constraint)
	{
		m_Constraints.erase(std::remove(m_Constraints.begin(), m_Constraints.end(), constraint), m_Constraints.end());
	}

	void Scene::SetPrimaryCamera(Ref<Entity> entity)
	{
		m_CameraSystem->SetPrimary(entity->GetID());
	}

	void Scene::SetPrimaryCamera(EntityID entity)
	{
		m_CameraSystem->SetPrimary(entity);
	}
}