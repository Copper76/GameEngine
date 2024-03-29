#include "EditorLayer.h"

namespace Fengshui
{
	EditorLayer::EditorLayer() : Layer("Sandbox2D")
	{

	}

	void EditorLayer::OnUpdate(float dt)
	{
		Ref<RenderComponent2D> renderComp = m_BigSquare->GetComponent<RenderComponent2D>();
		renderComp->SetColour(m_SquareColour);
		renderComp->SetTilingFactor(m_TilingFactor);
		Ref<TransformComponent2D> trans = m_BigSquare->GetComponent<TransformComponent2D>();
		trans->Rotation += dt * 10.0f;

		m_Framebuffer->Bind();
		m_Scene->OnUpdate(dt);
		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnAttach()
	{
		m_Scene = Scene::Init();

		m_Scene->GetCameraComponent()->SetZoomLevel(5.0f);

		m_BigSquare = GameEntity::Create(m_Scene);

		Ref<TransformComponent2D> trans = m_BigSquare->AddComponent<TransformComponent2D>();
		//trans->Position = { 0.5f, 0.0f };
		trans->Rotation = 45.0f;
		trans->Scale = { 5.0f, 5.0f };

		Ref<RenderComponent2D> renderComp = m_BigSquare->AddComponent<RenderComponent2D>(Texture2D::Create("Assets/Textures/Checkerboard.png"));
		//Ref<RenderComponent2D> renderComp = m_BigSquare->AddComponent<RenderComponent2D>();

		//Ref<RenderComponent2D> renderComp = m_BigSquare->AddComponent<RenderComponent2D>();
		//renderComp->SetTexture(Texture2D::Create("Assets/Textures/ChernoLogo.png"));

		FramebufferSpec spec;
		spec.Width = 1280;
		spec.Height = 720;
		m_Framebuffer = Framebuffer::Create(spec);
	}

	void EditorLayer::OnDetach()
	{

	}
}
