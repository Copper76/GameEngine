#include "EditorLayer.h"

#include <cmath>

namespace Fengshui
{
	EditorLayer::EditorLayer() : Layer("Sandbox2D")
	{

	}

	void EditorLayer::OnUpdate(float dt)
	{
		if (m_ActiveScene == m_Scene)
		{
			Render2D& renderComp = m_BigSquare->GetComponent<Render2D>();
			renderComp.Colour = m_SquareColour;
			renderComp.TilingFactor = m_TilingFactor;
			Transform2D& trans = m_BigSquare->GetComponent<Transform2D>();
			trans.Rotation = std::fmod(trans.Rotation + dt * 10.0f, 360.0f);
		}
		else
		{
			for (Ref<Entity> square : m_BackgroundSquares)
			{
				Transform2D& squareTrans = square->GetComponent<Transform2D>();
				squareTrans.Rotation = std::fmod(squareTrans.Rotation + dt * 10.0f, 360.0f);
			}
		}

		m_Framebuffer->Bind();
		m_ActiveScene->OnUpdate(dt);
		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnAttach()
	{
		m_Scene = Scene::Init();

		m_Scene->SetZoomLevel(5.0f);

		m_ActiveScene = m_Scene;

		m_SecondCamera = std::make_shared<Entity>("Second Camera");
		m_SecondCamera->AddComponent<CameraComponent>();

		m_BigSquare = std::make_shared<Entity>("Big Square");

		m_BigSquare->AddComponent<Transform2D>(Transform2D{ {0.0, 0.0} });

		glm::vec2 coords[] = { {0.0f, 0.0f}, {2.0f, 0.0f}, {2.0f, 1.0f}, {0.0f, 1.0f} };

		m_BigSquare->AddComponent(Render2D{ Texture2D::Create("Assets/Textures/Checkerboard.png"),
			coords
			});

		m_OtherScene = Scene::Init();

		Ref<Entity> square;
		for (float i = -5.0f; i < 5.0f; i += 1.0f)
		{
			for (float j = -5.0f; j < 5.0f; j += 1.0f)
			{
				square = std::make_shared<Entity>("("+std::to_string(i) + "," + std::to_string(j)+")");
				square->AddComponent<Transform2D>(Transform2D{ { i, j}, -0.5f, 45.0f, { 0.5f, 0.5f } });
				square->AddComponent(Render2D{ {(i + 5.0f) / 10.0f, (j + 5.0f) / 10.0f, 1.0f, 1.0f } });
				m_BackgroundSquares.emplace_back(square);
				//Renderer2D::DrawQuad({ i, j, -0.5f }, { 0.5f, 0.5f }, 45.0f, 1.0f, nullptr, defaultCoords, { (i + 0.5f) / 10.0f, (j + 0.5f) / 10.0f, 1.0f, 1.0f });
			}
		}

		FramebufferSpec spec;
		spec.Width = 1280;
		spec.Height = 720;
		m_Framebuffer = Framebuffer::Create(spec);

		m_SceneHierarchyPanel = std::make_shared<SceneHierarchyPanel>();
		GeneralManager::SetActiveScene(m_ActiveScene);
	}

	void EditorLayer::OnDetach()
	{

	}
}
