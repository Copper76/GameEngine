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

	m_BigSquare->AddComponent<Fengshui::TransformComponent2D>();

	m_BigSquare->AddComponent<Fengshui::RenderComponent2D>();
}

void Sandbox2D::OnDetach()
{

}
