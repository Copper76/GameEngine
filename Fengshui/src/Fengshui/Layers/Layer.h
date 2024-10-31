#pragma once

#include "Fengshui/Core/Core.h"
#include "Fengshui/Events/Event.h"

namespace Fengshui
{
	class Application;

	class Layer
	{
	public:
		Layer(const std::string& name = "Layer", bool isActive = true);
		virtual ~Layer() = default;

		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
		virtual void OnUpdate(float dt) {}
		virtual void OnFixedUpdate(float dt) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}
		inline void SetActive(bool isActive) { m_IsActive = isActive; }
		inline void SetApplication(Application* application) { m_Application = application; }

		inline const std::string& GetName() const { return m_DebugName; }
		inline bool IsActive() { return m_IsActive; }

	protected:
		std::string m_DebugName;
		bool m_IsActive;
		Application* m_Application;
	};

}
