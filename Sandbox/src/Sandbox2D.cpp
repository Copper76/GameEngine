#include "Sandbox2D.h"

Sandbox2D::Sandbox2D() : Fengshui::Layer("Sandbox2D")
{

}

void Sandbox2D::OnUpdate(float dt)
{
	m_Scene->OnUpdate(dt);
}

void Sandbox2D::OnAttach()
{
	m_Scene = Fengshui::Scene::Init();

	m_BigSquare = Fengshui::GameEntity::Create(m_Scene);

	Fengshui::Ref<Fengshui::TransformComponent2D> trans = m_BigSquare->AddComponent<Fengshui::TransformComponent2D>();
	//trans->Position = { 0.5f, 0.0f };
	//trans->Rotation = 45.0f;
	//trans->Scale = { 0.5f, 1.5f };

	m_BigSquare->AddComponent<Fengshui::RenderComponent2D>();
}

void Sandbox2D::OnDetach()
{

}
