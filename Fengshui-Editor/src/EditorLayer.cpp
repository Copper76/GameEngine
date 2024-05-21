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
				Render& renderComp = m_BigSquare->GetComponent<Render>();
				renderComp.Colour = m_SquareColour;
				renderComp.TilingFactor = m_TilingFactor;
				Transform& trans = m_BigSquare->GetComponent<Transform>();
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
		}
		if (m_EditorReady)
		{
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
		m_EditorReady = false;

		glm::vec2 coords[] = { {0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f},{0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f} };

		GeneralManager::Reset();
		m_Scene = Scene::Init();

		m_Scene->SetZoomLevel(5.0f);

		m_ActiveScene = m_Scene;

		m_SecondCamera = std::make_shared<Entity>("Second Camera");
		m_SecondCamera->AddComponent<CameraComponent>();
		m_SecondCamera->AddComponent<Transform>();

		////stacking box
		//for (int y = 0; y < 5; y++) {
		//	Ref<Entity> box = std::make_shared<Entity>("Box");
		//	float offset = ((y & 1) == 0) ? 0.0f : 0.15f;//offset even-numbered boxes
		//	float delta = 0.04f;
		//	float scaleHeight = 2.0f + delta;
		//	float deltaHeight = 1.0f + delta;
		//	box->AddComponent<Transform>(glm::vec3(offset * scaleHeight, deltaHeight + (float)y * scaleHeight, offset * scaleHeight));
		//	box->AddComponent<Rigidbody>();
		//	box->AddComponent<Collider>();
		//	box->AddComponent<Render>(Render{ nullptr,
		//	coords
		//		});
		//	m_StackingBoxes.push_back(box);
		//}

		m_BigSquare = std::make_shared<Entity>("Big Square");

		Transform bigSquareTrans = m_BigSquare->AddComponent<Transform>(Transform(glm::vec3(1.0f, 2.0f, 0.0f)));

		m_BigSquare->AddComponent<Render>(Render{ nullptr,
			coords
			});

		m_BigSquare->AddComponent<Rigidbody>();
		Collider bigSquareCollider = m_BigSquare->AddComponent<Collider>();

		m_Ground = std::make_shared<Entity>("Ground");

		Transform groundTrans = m_Ground->AddComponent<Transform>(Transform(glm::vec3(0.0f, -2.0f, 0.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(5.0f, 1.0f, 1.0f)));

		m_Ground->AddComponent<Render>(Render{ nullptr,
			coords
			});

		m_Ground->AddComponent<Rigidbody>(Rigidbody(0.0f));
		Collider groundCollider = m_Ground->AddComponent<Collider>();

		m_BigSquare->SetParent(m_SecondCamera);

		//ConstraintDistance* joint = new ConstraintDistance();

		//const glm::vec3 jointWorldSpaceAnchor = groundTrans.Position;

		//joint->m_BodyA = m_BigSquare->GetID();
		//joint->m_anchorA = WorldSpaceToBodySpace(jointWorldSpaceAnchor, bigSquareCollider, bigSquareTrans);

		//joint->m_BodyB = m_Ground->GetID();
		//joint->m_anchorB = WorldSpaceToBodySpace(jointWorldSpaceAnchor, groundCollider, groundTrans);
		//m_Scene->AddConstraint(joint);

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
		m_EditorReady = true;
	}

	void EditorLayer::OnDetach()
	{

	}
}
