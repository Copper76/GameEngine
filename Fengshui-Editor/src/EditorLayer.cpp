#include "EditorLayer.h"

#include <cmath>

namespace Fengshui
{
	EditorLayer::EditorLayer() : Layer("Sandbox2D")
	{

	}

	void EditorLayer::OnUpdate(float dt)
	{
		/**
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
		**/
		if (Input::IsKeyPressed(FS_KEY_A) && m_PrevDir != 1)
		{
			m_SnakeDir = 3;
		}
		if (Input::IsKeyPressed(FS_KEY_D) && m_PrevDir != 3)
		{
			m_SnakeDir = 1;
		}
		if (Input::IsKeyPressed(FS_KEY_S) && m_PrevDir != 0)
		{
			m_SnakeDir = 2;
		}
		if (Input::IsKeyPressed(FS_KEY_W) && m_PrevDir != 2)
		{
			m_SnakeDir = 0;
		}

		if (dt > m_RemainingUpdateTime && !m_GameOver)
		{
			m_PrevDir = m_SnakeDir;
			switch (m_SnakeDir)
			{
			case 0:
				m_SnakeHeadPos = m_SnakeHeadPos + glm::vec2(0.0f, 1.0f);
				break;
			case 1:
				m_SnakeHeadPos = m_SnakeHeadPos + glm::vec2(1.0f, 0.0f);
				break;
			case 2:
				m_SnakeHeadPos = m_SnakeHeadPos - glm::vec2(0.0f, 1.0f);
				break;
			case 3:
				m_SnakeHeadPos = m_SnakeHeadPos - glm::vec2(1.0f, 0.0f);
				break;
			default:
				break;
			}

			if ((m_SnakeHeadPos.x < -(m_BoardSize - 1) / 2 || m_SnakeHeadPos.x > (m_BoardSize - 1)/2 || m_SnakeHeadPos.y < -(m_BoardSize - 1) / 2 || m_SnakeHeadPos.y > (m_BoardSize - 1) / 2 || m_SnakeBody.size() >= m_BoardSize * m_BoardSize || std::find(m_SnakeBodyPos.begin(), m_SnakeBodyPos.end(), m_SnakeHeadPos) != m_SnakeBodyPos.end()) && !m_GameOver)
			{
				m_GameOver = true;
			}
			else
			{
				if (m_SnakeHeadPos == m_FruitPos)
				{
					Ref<Entity> tail = std::make_shared<Entity>("Snake");
					tail->AddComponent<Render2D>(m_SnakeColour);
					tail->AddComponent<Transform2D>(Transform2D(glm::vec3(m_SnakeHeadPos, 1.0f)));
					tail->SetParent(m_SnakeContainer);
					m_SnakeBodyPos.emplace_back(m_SnakeHeadPos);
					m_SnakeBody.emplace_back(tail);
					do
					{
						m_FruitPos = glm::vec2((rand() % m_BoardSize) - 5, (rand() % m_BoardSize) - 5);
					} while (std::find(m_SnakeBodyPos.begin(), m_SnakeBodyPos.end(), m_FruitPos) != m_SnakeBodyPos.end() && m_SnakeBody.size() < m_BoardSize * m_BoardSize);
					m_Fruit->GetComponent<Transform2D>().Position = glm::vec3(m_FruitPos, 1.0f);
				}
				else
				{
					Ref<Entity> tail = m_SnakeBody.front();
					m_SnakeBody.pop_front();
					m_SnakeBodyPos.pop_front();
					tail->GetComponent<Transform2D>().Position = glm::vec3(m_SnakeHeadPos, 1.0f);
					m_SnakeBody.push_back(tail);
					m_SnakeBodyPos.push_back(m_SnakeHeadPos);
				}
			}

			m_RemainingUpdateTime = m_UpdateInterval;
		}
		else
		{
			m_RemainingUpdateTime -= dt;
		}

		m_Framebuffer->Bind();
		m_ActiveScene->OnUpdate(dt);
		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnAttach()
	{
		m_Scene = Scene::Init();

		m_Scene->SetZoomLevel(5.5f);

		m_ActiveScene = m_Scene;
		/**
		m_SecondCamera = std::make_shared<Entity>("Second Camera");
		m_SecondCamera->AddComponent<CameraComponent>();
		m_SecondCamera->AddComponent<Transform2D>();

		m_BigSquare = std::make_shared<Entity>("Big Square");

		auto& comp = GeneralManager::GetComponent<Hierarchy>(0);

		m_BigSquare->AddChild(m_SecondCamera);

		comp = GeneralManager::GetComponent<Hierarchy>(0);

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
		**/

		//Snake Setup
		m_BoardBackground = std::make_shared<Entity>("Board Background");
		m_BoardBackground->AddComponent<Render2D>();
		m_BoardBackground->AddComponent<Transform2D>(Transform2D(glm::vec3(0.0f), 0.0f, glm::vec2(m_BoardSize)));

		m_SnakeContainer = std::make_shared<Entity>("Snake Body");

		Ref<Entity> snake = std::make_shared<Entity>("Snake");
		snake->AddComponent<Render2D>(m_SnakeColour);
		snake->AddComponent<Transform2D>(Transform2D(glm::vec3(m_SnakeHeadPos, 1.0f)));
		snake->SetParent(m_SnakeContainer);
		m_SnakeBodyPos.emplace_back(m_SnakeHeadPos);
		m_SnakeBody.emplace_back(snake);

		m_Fruit = std::make_shared<Entity>("Fruit");
		m_Fruit->AddComponent<Render2D>(m_FruitColour);
		do
		{
			m_FruitPos = glm::vec2((rand() % m_BoardSize)-5, (rand() % m_BoardSize) - 5);
		} while (std::find(m_SnakeBodyPos.begin(), m_SnakeBodyPos.end(), m_FruitPos) != m_SnakeBodyPos.end() && m_SnakeBody.size() < m_BoardSize * m_BoardSize);
		m_Fruit->AddComponent<Transform2D>(Transform2D(glm::vec3(m_FruitPos, 1.0f), 0.0f, glm::vec2(0.5f)));

		//Setup framebuffer
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
