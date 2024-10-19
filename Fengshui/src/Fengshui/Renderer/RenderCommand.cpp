#include "fspch.h"
#include "Fengshui/Renderer/RenderCommand.h"

#include "Fengshui/Platform/OpenGL/OpenGLRendererAPI.h"

namespace Fengshui
{
	Scope<RendererAPI> RenderCommand::s_RendererAPI = MakeScope<OpenGLRenderer>();
}