#pragma once

#ifdef FS_PLATFORM_WINDOWS
	#ifdef FS_BUILD_DLL
		#define FENGSHUI_API __declspec(dllexport)
	#else
		#define FENGSHUI_API __declspec(dllimport)
	#endif
#else
	#error Fengshui only supports Windows!
#endif