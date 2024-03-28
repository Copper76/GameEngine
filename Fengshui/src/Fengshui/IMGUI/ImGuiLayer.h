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

	private:
		float m_Time = 0.0f;
	};

}
