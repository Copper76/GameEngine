#include "fspch.h"
#include "Fengshui/Renderer/RenderCommand.h"

#include "Fengshui/Platform/OpenGL/OpenGLRendererAPI.h"

namespace Fengshui
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRenderer;
}