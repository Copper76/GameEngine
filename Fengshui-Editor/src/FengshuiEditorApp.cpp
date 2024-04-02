#include <Fengshui.h>
#include "EditorLayer.h"

#include <Fengshui/Core/EntryPoint.h>

//External includes
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Fengshui
{
	class FengshuiEditor : public Application
	{
	public:
		FengshuiEditor() : Application("Fengshui Editor")
		{
			PushLayer(new EditorLayer());
		}

		~FengshuiEditor()
		{

		}
	};

	Application* CreateApplication()
	{
		return new FengshuiEditor();
	}
}