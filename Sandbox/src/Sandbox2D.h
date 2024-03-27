#pragma once
#include "Fengshui.h"

//External includes
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

class Sandbox2D : public Fengshui::Layer
{
public:
	Sandbox2D();

	void OnUpdate(float dt) override;

	void OnAttach() override;
	void OnDetach() override;

	void OnImGuiRender() override
	{
		ImGui::Begin("Colour Picker");
		ImGui::ColorEdit4("Square Colour", glm::value_ptr(m_SquareColour));
		ImGui::DragFloat("Tiling Factor", &m_TilingFactor);
		ImGui::End();
	}

	void OnEvent(Fengshui::Event& e) override
	{
		m_Scene->OnEvent(e);
	}

private:
	Fengshui::Ref<Fengshui::Scene> m_Scene;
	glm::vec4 m_SquareColour = { 1.0f, 1.0f, 1.0f, 1.0f };
	float m_TilingFactor = 1.0f;

	Fengshui::Ref < Fengshui::GameEntity> m_BigSquare;
};