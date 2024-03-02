#pragma once

#include <memory>

#include "Fengshui/Core.h"
#include "spdlog/spdlog.h"

namespace Fengshui
{
	class FENGSHUI_API Log
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger>& GetEngineLogger() { return s_EngineLogger; }
		static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_EngineLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

//Engine logging
#define FS_ENGINE_FATAL(...) ::Fengshui::Log::GetEngineLogger()->fatal(__VA_ARGS__)
#define FS_ENGINE_ERROR(...) ::Fengshui::Log::GetEngineLogger()->error(__VA_ARGS__)
#define FS_ENGINE_WARN(...) ::Fengshui::Log::GetEngineLogger()->warn(__VA_ARGS__)
#define FS_ENGINE_INFO(...) ::Fengshui::Log::GetEngineLogger()->info(__VA_ARGS__)
#define FS_ENGINE_TRACE(...) ::Fengshui::Log::GetEngineLogger()->trace(__VA_ARGS__)

//Client logging
#define FS_FATAL(...) ::Fengshui::Log::GetClientLogger()->fatal(__VA_ARGS__)
#define FS_ERROR(...) ::Fengshui::Log::GetClientLogger()->error(__VA_ARGS__)
#define FS_WARN(...) ::Fengshui::Log::GetClientLogger()->warn(__VA_ARGS__)
#define FS_INFO(...) ::Fengshui::Log::GetClientLogger()->info(__VA_ARGS__)
#define FS_TRACE(...) ::Fengshui::Log::GetClientLogger()->trace(__VA_ARGS__)