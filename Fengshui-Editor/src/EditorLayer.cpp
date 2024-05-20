#include "EditorLayer.h"

#include <cmath>

namespace Fengshui
{
	EditorLayer::EditorLayer() : Layer("Sandbox2D")
	{

	}

	void EditorLayer::OnUpdate(float dt)
	{
		if (m_IsPlaying && !m_Paused)
		{

			if (m_ActiveScene == m_Scene)
			{
				//Render2D& renderComp = m_BigSquare->GetComponent<Render2D>();
				Render& renderComp = m_BigSquare->GetComponent<Render>();
				renderComp.Colour = m_SquareColour;
				renderComp.TilingFactor = m_TilingFactor;
				//Transform2D& trans = m_BigSquare->GetComponent<Transform2D>();
				//Transform& trans = m_BigSquare->GetComponent<Transform>();
				//glm::quat& rotateDelta = glm::normalize(glm::quat(glm::radians(glm::vec3(dt * 0.01f, dt * 0.01f, 0.0f))));
				//trans.Rotation *= rotateDelta;
			}
			else
			{
				for (Ref<Entity> square : m_BackgroundSquares)
				{
					Transform2D& squareTrans = square->GetComponent<Transform2D>();
					squareTrans.Rotation = std::fmod(squareTrans.Rotation + dt * 10.0f, 360.0f);
				}
			}

			m_ActiveScene->OnUpdate(dt);
		}
	}

	void EditorLayer::OnFixedUpdate(float dt)
	{
		if (m_IsPlaying && !m_Paused)
		{
			m_ActiveScene->OnFixedUpdate(dt);
		}
	}

	void EditorLayer::OnRender()
	{
		m_Framebuffer->Bind();
		m_ActiveScene->OnRender();
		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnAttach()
	{
		FramebufferSpec spec;
		spec.Width = 1280;
		spec.Height = 720;
		m_Framebuffer = Framebuffer::Create(spec);

		m_SceneHierarchyPanel = std::make_shared<SceneHierarchyPanel>();

		Reset();
	}

	void EditorLayer::Reset()
	{
		GeneralManager::Reset();
		m_Scene = Scene::Init();

		m_Scene->SetZoomLevel(5.0f);

		m_ActiveScene = m_Scene;

		m_SecondCamera = std::make_shared<Entity>("Second Camera");
		m_SecondCamera->AddComponent<CameraComponent>();
		m_SecondCamera->AddComponent<Transform>();

		m_BigSquare = std::make_shared<Entity>("Big Square");

		//m_BigSquare->AddComponent<Transform2D>();
		m_BigSquare->AddComponent<Transform>();

		glm::vec2 coords[] = { {0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f},{0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f} };

		m_BigSquare->AddComponent<Render>(Render{ nullptr,
			coords
			});

		m_BigSquare->AddComponent<Rigidbody>();
		m_BigSquare->AddComponent<Collider>();

		m_Ground = std::make_shared<Entity>("Ground");

		m_Ground->AddComponent<Transform>(Transform(glm::vec3(0.0f, -2.0f, 0.0f)));

		m_Ground->AddComponent<Render>(Render{ nullptr,
			coords
			});

		m_Ground->AddComponent<Rigidbody>(Rigidbody(0.0f));
		m_Ground->AddComponent<Collider>();

		m_BigSquare->SetParent(m_SecondCamera);

		m_OtherScene = Scene::Init();
		
		Ref<Entity> square;
		for (float i = -5.0f; i < 5.0f; i += 1.0f)
		{
			for (float j = -5.0f; j < 5.0f; j += 1.0f)
			{
				square = std::make_shared<Entity>("(" + std::to_string(i) + "," + std::to_string(j) + ")");
				square->AddComponent<Transform2D>(Transform2D{ { i, j}, -0.5f, 45.0f, { 0.5f, 0.5f } });
				square->AddComponent(Render2D{ {(i + 5.0f) / 10.0f, (j + 5.0f) / 10.0f, 1.0f, 1.0f } });
				m_BackgroundSquares.emplace_back(square);
				//Renderer2D::DrawQuad({ i, j, -0.5f }, { 0.5f, 0.5f }, 45.0f, 1.0f, nullptr, defaultCoords, { (i + 0.5f) / 10.0f, (j + 0.5f) / 10.0f, 1.0f, 1.0f });
			}
		}
		
		GeneralManager::SetActiveScene(m_ActiveScene);
	}

	void EditorLayer::OnDetach()
	{

	}
}
