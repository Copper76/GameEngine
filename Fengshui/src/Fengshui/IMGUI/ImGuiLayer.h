#pragma once

#include "Fengshui/Layers/Layer.h"

#include "Fengshui/Events/ApplicationEvent.h"
#include "Fengshui/Events/KeyEvent.h"
#include "Fengshui/Events/MouseEvent.h"


namespace Fengshui
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		//virtual void OnUpdate();
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

		/**
		void OnEvent(Event& event);

	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);

		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e);

		bool OnWindowResizeEvent(WindowResizeEvent& e);
		**/

	private:
		float m_Time = 0.0f;
	};

}
