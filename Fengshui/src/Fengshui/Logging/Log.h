#pragma once

#include "Fengshui/Core/Core.h"
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Fengshui
{
	class Log
	{
	public:
		static void Init();

		static Ref<spdlog::logger> GetEngineLogger() { return s_EngineLogger; }
		static Ref<spdlog::logger> GetClientLogger() { return s_ClientLogger; }

	private:
		static Ref<spdlog::logger> s_EngineLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};
}

//FS_ENGINE_ERROR("{0}", bla bla) for placeholder

//Engine logging
#define FS_ENGINE_CRITICAL(...) ::Fengshui::Log::GetEngineLogger()->critical(__VA_ARGS__);
#define FS_ENGINE_ERROR(...)    ::Fengshui::Log::GetEngineLogger()->error(__VA_ARGS__);
#define FS_ENGINE_WARN(...)     ::Fengshui::Log::GetEngineLogger()->warn(__VA_ARGS__);
#define FS_ENGINE_INFO(...)     ::Fengshui::Log::GetEngineLogger()->info(__VA_ARGS__);
#define FS_ENGINE_TRACE(...)    ::Fengshui::Log::GetEngineLogger()->trace(__VA_ARGS__);

//Client logging

#define FS_CRITICAL(...) ::Fengshui::Log::GetClientLogger()->critical(__VA_ARGS__);
#define FS_ERROR(...) ::Fengshui::Log::GetClientLogger()->error(__VA_ARGS__);
#define FS_WARN(...) ::Fengshui::Log::GetClientLogger()->warn(__VA_ARGS__);
#define FS_INFO(...) ::Fengshui::Log::GetClientLogger()->info(__VA_ARGS__);
#define FS_TRACE(...) ::Fengshui::Log::GetClientLogger()->trace(__VA_ARGS__);

/**
#define FS_CRITICAL(...) std::cout<< __VA_ARGS__ << std::endl;
#define FS_ERROR(...) std::cout<< __VA_ARGS__ << std::endl;
#define FS_WARN(...) std::cout<< __VA_ARGS__ << std::endl;
#define FS_INFO(...) std::cout<< __VA_ARGS__ << std::endl;
#define FS_TRACE(...) std::cout<< __VA_ARGS__ << std::endl;

#define FS_CRITICAL(...)
#define FS_ERROR(...)
#define FS_WARN(...)
#define FS_INFO(...)
#define FS_TRACE(...)
**/
