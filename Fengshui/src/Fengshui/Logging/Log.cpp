#include "fspch.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Fengshui
{
	std::shared_ptr<spdlog::logger> Log::s_EngineLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$"); // "%^[%T] %g %# %n: %v%$"
		
		s_EngineLogger = spdlog::stdout_color_mt("FENGSHUI");
		s_EngineLogger->set_level(spdlog::level::level_enum::trace);

		s_ClientLogger = spdlog::stdout_color_mt("CLIENT");
		s_ClientLogger->set_level(spdlog::level::level_enum::trace);
	}
}