#pragma once

#include "Fengshui/Core.h"
#include "Fengshui/Events/Event.h"

namespace Fengshui
{
	class FENGSHUI_API Layer
	{
	public:
		Layer(const std::string& name = "Layer", bool isActive = true);
		virtual ~Layer();

		//Need implementation
		virtual void OnAttach() = 0;
		virtual void OnDettach() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
		bool m_IsActive;
	};

}
