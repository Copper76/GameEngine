#pragma once

namespace Fengshui
{
	enum class RenderAPI
	{
		None = 0, OpenGL = 1,
	};

	class Renderer
	{
	public:
		inline static RenderAPI GetAPI() { return s_RenderAPI; }
		inline static void SetAPI(RenderAPI api) { s_RenderAPI = api; }

	private:
		static RenderAPI s_RenderAPI;
	};
}