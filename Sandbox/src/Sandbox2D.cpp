#include "Sandbox2D.h"

Sandbox2D::Sandbox2D() : Fengshui::Layer("Sandbox2D")
{

}

void Sandbox2D::OnUpdate(float dt)
{
	/**
	Fengshui::Ref<Fengshui::RenderComponent2D> renderComp = m_BigSquare->GetComponent<Fengshui::RenderComponent2D>();
	renderComp->SetColour(m_SquareColour);
	renderComp->SetTilingFactor(m_TilingFactor);
	Fengshui::Ref<Fengshui::TransformComponent2D> trans = m_BigSquare->GetComponent<Fengshui::TransformComponent2D>();
	trans->Rotation += dt * 10.0f;
	**/
	m_Scene->OnUpdate(dt);
}

void Sandbox2D::OnAttach()
{
	m_Scene = Fengshui::Scene::Init();
	/**
	m_BigSquare = Fengshui::GameEntity::Create(m_Scene);

	Fengshui::Ref<Fengshui::TransformComponent2D> trans = m_BigSquare->AddComponent<Fengshui::TransformComponent2D>();
	//trans->Position = { 0.5f, 0.0f };
	trans->Rotation = 45.0f;
	trans->Scale = { 5.0f, 5.0f };

	Fengshui::Ref<Fengshui::RenderComponent2D> renderComp = m_BigSquare->AddComponent<Fengshui::RenderComponent2D>(Fengshui::Texture2D::Create("Assets/Textures/Checkerboard.png"));
	**/
	//Fengshui::Ref<Fengshui::RenderComponent2D> renderComp = m_BigSquare->AddComponent<Fengshui::RenderComponent2D>();

	//Fengshui::Ref<Fengshui::RenderComponent2D> renderComp = m_BigSquare->AddComponent<Fengshui::RenderComponent2D>();
	//renderComp->SetTexture(Fengshui::Texture2D::Create("Assets/Textures/ChernoLogo.png"));
}

void Sandbox2D::OnDetach()
{

}
