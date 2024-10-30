#include "EditorLayer.h"

#include <cmath>

namespace Fengshui
{
	//Creating the editor layer
	EditorLayer::EditorLayer() : Layer("Sandbox")
	{

	}

	//Update function called for editor, you can also treat this as game update
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
					squareTrans.Rotation = std::fmod(squareTrans.Rotation + dt, 360.0f);
				}
			}
		}
		if (m_EditorReady)
		{
			m_ActiveScene->OnUpdate(dt);
		}
	}

	//Fixed update code for the editor, it just simply call the scene update for now
	void EditorLayer::OnFixedUpdate(float dt)
	{
		if (m_IsPlaying && !m_Paused && m_EditorReady)
		{
			m_ActiveScene->OnFixedUpdate(dt);
		}
	}

	//Render code for editor, if the application is a game, no framebuffer will be required
	void EditorLayer::OnRender()
	{
		if (m_EditorReady)
		{
			m_Framebuffer->Bind();
			m_ActiveScene->OnRender();
			m_Framebuffer->Unbind();
		}
	}

	//Code run when the layer is attached to the layer stack
	void EditorLayer::OnAttach()
	{
		//Creates a framebuffer to fit into imgui
		FramebufferSpec spec;
		spec.Width = 1280;
		spec.Height = 720;
		m_Framebuffer = Framebuffer::Create(spec);

		m_SceneHierarchyPanel = MakeRef<SceneHierarchyPanel>();

		Reset();
	}

	//Code to reset the scene
	void EditorLayer::Reset()
	{
		m_EditorReady = false;

		GeneralManager::Reset();
		m_Scene = Scene::Init();

		m_Scene->SetZoomLevel(5.0f);

		m_ActiveScene = m_Scene;

		m_SecondCamera = MakeRef<Entity>("Second Camera");
		m_SecondCamera->AddComponent<CameraComponent>();
		m_SecondCamera->AddComponent<Transform>();
		//m_SecondCamera->AddComponent<Light>(Light(LightType::PointLight));

		////stacking box
		//for (int y = 0; y < 5; y++) {
		//	Ref<Entity> box = MakeRef<Entity>("Box");
		//	float offset = ((y & 1) == 0) ? 0.0f : 0.15f;//offset even-numbered boxes
		//	float delta = 0.04f;
		//	float scaleHeight = 2.0f + delta;
		//	float deltaHeight = 1.0f + delta;
		//	box->AddComponent<Transform>(glm::vec3(offset * scaleHeight, deltaHeight + (float)y * scaleHeight, offset * scaleHeight));
		//	box->AddComponent<Rigidbody>();
		//	box->AddComponent<Collider>();
		//	box->AddComponent<Render>();
		//	m_StackingBoxes.push_back(box);
		//}

		m_2DSquare = MakeRef<Entity>("2D Square");

		m_2DSquare->AddComponent<Transform2D>();
		m_2DSquare->AddComponent<Render2D>();


		m_BigSquare = MakeRef<Entity>("Big Square");

		Transform bigSquareTrans = m_BigSquare->AddComponent<Transform>(Transform(glm::vec3(1.0f, 2.0f, 0.0f)));

		glm::vec3 pts[] =
		{ glm::vec3(-0.5,-0.5,-0.5),
		glm::vec3(0.5, -0.5,-0.5),
		glm::vec3(0.5, -0.5,0.5),
		glm::vec3(-0.5, -0.5,0.5),
		glm::vec3(0,0.5,0), };
		RenderShapeConvex* convexShape = new RenderShapeConvex(pts, sizeof(pts) / sizeof(glm::vec3));

		Ref<Texture> checkerboard = Texture::Create("E:/GitStuff/GameEngine/Fengshui-Editor/Assets/Textures/Checkerboard.png");

		//m_BigSquare->AddComponent<Render>(Render(convexShape));
		//m_BigSquare->AddComponent<Render>();
		m_BigSquare->AddComponent<Render>(Render(new RenderShapeCube(), checkerboard));

		m_AudioSource = m_BigSquare->AddComponent<AudioSourceComponent>(AudioSourceComponent(AudioCommand::CreateAudioSource("Assets/AudioClip/bell.wav")));

		m_BigSquare->AddComponent<Rigidbody>();
		Collider bigSquareCollider = m_BigSquare->AddComponent<Collider>();

		m_Ground = MakeRef<Entity>("Ground");

		Transform groundTrans = m_Ground->AddComponent<Transform>(Transform(glm::vec3(0.0f, -2.0f, 0.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(5.0f, 1.0f, 5.0f)));

		m_Ground->AddComponent<Render>(Render(checkerboard));

		m_Ground->AddComponent<Rigidbody>(Rigidbody(0.0f));
		Collider groundCollider = m_Ground->AddComponent<Collider>(Collider(new PhysicalShapeBox(glm::vec3(0.0f), TransformSystem::GetWorldTransform(m_Ground->GetID()).Scale)));

		//m_BigSquare->SetParent(m_SecondCamera);

		//Ref<ConstraintDistance> joint = MakeRef<ConstraintDistance>();

		//const glm::vec3 jointWorldSpaceAnchor = groundTrans.Position;

		//joint->m_BodyA = m_BigSquare->GetID();
		//joint->m_anchorA = WorldSpaceToBodySpace(jointWorldSpaceAnchor, bigSquareCollider, bigSquareTrans);

		//joint->m_BodyB = m_Ground->GetID();
		//joint->m_anchorB = WorldSpaceToBodySpace(jointWorldSpaceAnchor, groundCollider, groundTrans);
		//m_Scene->AddConstraint(joint);

		m_OtherScene = Scene::Init();
		/*m_AudioSource2->SetSetting(AudioSetting{ true });*/
		
		Ref<Entity> square;
		for (float i = -5.0f; i < 5.0f; i += 1.0f)
		{
			for (float j = -5.0f; j < 5.0f; j += 1.0f)
			{
				square = MakeRef<Entity>("(" + std::to_string(i) + "," + std::to_string(j) + ")");
				square->AddComponent<Transform2D>(Transform2D{ { i, j}, -0.5f, 45.0f, { 0.5f, 0.5f } });
				square->AddComponent(Render2D{ {(i + 5.0f) / 10.0f, (j + 5.0f) / 10.0f, 1.0f, 1.0f } });
				m_BackgroundSquares.emplace_back(square);
			}
		}
		
		GeneralManager::SetActiveScene(m_ActiveScene);

		glm::quat quat = glm::angleAxis(1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		quat = glm::normalize(quat);
		glm::mat3 tensor = glm::mat3_cast(quat);

		//glm::mat3 tensor = glm::transpose(glm::mat3(1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f));
		glm::vec3 vec = tensor[1];
		//glm::vec3 vec2 = tensor * vec;
		//vec2 = glm::cross(vec, vec2);
		//vec = glm::inverse(tensor) * vec2;
		//FS_INFO("Identity Quat: ({}, {}, {}, {})", quat.w, quat.x, quat.y, quat.z);
		FS_INFO("Identity Vec: ({}, {}, {})", vec.x, vec.y, vec.z);

		m_EditorReady = true;
	}

	void EditorLayer::OnDetach()
	{

	}
}