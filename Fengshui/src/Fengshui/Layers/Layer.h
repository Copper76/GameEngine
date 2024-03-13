#pragma once

#include "Fengshui/Core/Core.h"
#include "Fengshui/Events/Event.h"

namespace Fengshui
{
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer", bool isActive = true);
		virtual ~Layer() = default;

		//Need implementation
		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
		virtual void OnUpdate(float dt) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}
		void SetActive(bool isActive);

		inline const std::string& GetName() const { return m_DebugName; }
		inline bool IsActive() { return m_IsActive; }

	protected:
		std::string m_DebugName;
		bool m_IsActive;
	};

}
