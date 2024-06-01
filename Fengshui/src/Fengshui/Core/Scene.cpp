#include "fspch.h"
#include "Scene.h"

#include "Fengshui/Renderer/Renderer.h"
#include "Fengshui/Renderer/Renderer2D.h"
#include "Fengshui/ECS/GeneralManager.h"


namespace Fengshui
{
	//Default destructor
	Scene::~Scene()
	{

	}

	//Function for initialising a scene
	Ref<Scene> Scene::Init()
	{
		//set up the clear settings for the scene
		RenderCommand::SetClearColour({ 0.2f, 0.2f, 0.2f, 1 });
		RenderCommand::SetClearDepth(1.0f);

		//Create Scene
		auto scene = std::make_shared<Scene>();

		//Make the current scene active for the purpose of working with general manager
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
		signature.set(GeneralManager::GetComponentType<Hierarchy>());
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

		//Setup a default perspective camera
		scene->m_SceneManager->AddComponent(CameraComponent{
			false,
			true});

		scene->m_SceneManager->AddComponent<Transform>(Transform(glm::vec3(0.0f, 0.0f, 10.0f)));

		scene->UpdateViewMatrix(scene->m_CameraSystem->GetPrimary());

		return scene;
	}

	//Update function for the scene
	void Scene::OnUpdate(float dt)
	{
		m_PrimaryCamera = m_CameraSystem->GetPrimary();
		
		//A default camera control for navigating the scene
		if (m_PrimaryCamera != 0)
		{
			if (m_ViewportFocused)
			{
				glm::vec3 moveDelta = glm::vec3(0.0f);
				glm::vec3 rotateDelta = glm::vec3(0.0f);

				//Input handling
				if (Input::IsKeyPressed(FS_KEY_A))
				{
					moveDelta.x -= m_CameraMoveSpeed * dt;
				}
				if (Input::IsKeyPressed(FS_KEY_D))
				{
					moveDelta.x += m_CameraMoveSpeed * dt;
				}
				if (Input::IsKeyPressed(FS_KEY_SPACE))
				{
					moveDelta.y += m_CameraMoveSpeed * dt;
				}
				if (Input::IsKeyPressed(FS_KEY_LEFT_CONTROL))
				{
					moveDelta.y -= m_CameraMoveSpeed * dt;
				}
				if (Input::IsKeyPressed(FS_KEY_W))
				{
					moveDelta.z -= m_CameraMoveSpeed * dt;
				}
				if (Input::IsKeyPressed(FS_KEY_S))
				{
					moveDelta.z += m_CameraMoveSpeed * dt;
				}

				if (Input::IsKeyPressed(FS_KEY_I))
				{
					rotateDelta.x += m_CameraMoveSpeed * dt;
				}
				if (Input::IsKeyPressed(FS_KEY_K))
				{
					rotateDelta.x -= m_CameraMoveSpeed * dt;
				}
				if (Input::IsKeyPressed(FS_KEY_J))
				{
					rotateDelta.y += m_CameraMoveSpeed * dt;
				}
				if (Input::IsKeyPressed(FS_KEY_L))
				{
					rotateDelta.y -= m_CameraMoveSpeed * dt;
				}

				if (Input::IsKeyPressed(FS_KEY_Q))
				{
					rotateDelta.z += m_CameraMoveSpeed * dt;
				}
				if (Input::IsKeyPressed(FS_KEY_E))
				{
					rotateDelta.z -= m_CameraMoveSpeed * dt;
				}

				AdjustCamera(m_PrimaryCamera, moveDelta, glm::quat(glm::radians(rotateDelta)));
			}
		}
	}

	//Fixed update function for updating the gravity and physics of the scene
	void Scene::OnFixedUpdate(float dt)
	{
		//Update is conducted in two halves
		float dt_sec = dt * 0.5f;
		for (int i = 0; i < 2; i++)
		{
			m_GravitySystem->OnUpdate(dt_sec);
			m_PhysicsSystem->OnUpdate(dt_sec, m_Manifolds, m_Constraints);
		}
	}

	//Render function for updating the graphics of the scene
	void Scene::OnRender()
	{

		if (m_PrimaryCamera != 0)
		{
			//Clear the screen
			RenderCommand::Clear();

			//3D Render cycle
			Renderer::BeginScene(&GeneralManager::GetComponent<CameraComponent>(m_PrimaryCamera));

			m_RenderSystem->OnRender(m_TransformSystem);

			Renderer::EndScene();

			//2D Render cycle
			Renderer2D::BeginScene(&GeneralManager::GetComponent<CameraComponent>(m_PrimaryCamera));

			m_RenderSystem2D->OnRender(m_TransformSystem);

			Renderer2D::EndScene();
		}
	}

	//Event callback for the scene
	void Scene::OnEvent(Event& e)
	{
		EventDispatcher eventDispatcher(e);
		eventDispatcher.Dispatch<MouseScrolledEvent>(FS_BIND_EVENT_FN(Scene::OnMouseScrolled));
		eventDispatcher.Dispatch<WindowResizeEvent>(FS_BIND_EVENT_FN(Scene::OnWindowResize));
	}

	//Wrapper functions for the camera system
	bool Scene::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_CameraSystem->OnMouseScrolled(m_SceneManager->GetID(), e);
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

	void Scene::AdjustCamera(EntityID entity, glm::vec3 moveDelta, glm::quat rotateDelta)
	{
		m_CameraSystem->AdjustCamera(entity, moveDelta, rotateDelta, m_TransformSystem);
	}

	void Scene::UpdateView()
	{
		if (m_PrimaryCamera != 0)
		{
			m_CameraSystem->CalculateView(m_PrimaryCamera);
		}
	}

	void Scene::UpdateViewMatrix(EntityID entity)
	{
		m_CameraSystem->RecalculateViewMatrix(entity, m_TransformSystem);
	}

	void Scene::SetPrimaryCamera(Ref<Entity> entity)
	{
		m_CameraSystem->SetPrimary(entity->GetID());
	}

	void Scene::SetPrimaryCamera(EntityID entity)
	{
		m_CameraSystem->SetPrimary(entity);
	}

	//Wrapper for the handling constraints to be used in physics system
	void Scene::AddConstraint(Ref<Constraint> constraint)
	{
		m_Constraints.push_back(constraint);
	}

	void Scene::RemoveConstraint(Ref<Constraint> constraint)
	{
		m_Constraints.erase(std::remove(m_Constraints.begin(), m_Constraints.end(), constraint), m_Constraints.end());
	}
}