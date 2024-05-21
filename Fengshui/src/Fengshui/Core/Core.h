#pragma once

#include <memory>

#ifdef FS_PLATFORM_WINDOWS
	#ifdef FS_DYNAMIC_LINK
		#ifdef FS_BUILD_DLL
			#define FENGSHUI_API __declspec(dllexport)
		#else
			#define FENGSHUI_API __declspec(dllimport)
		#endif
	#else
		#define FENGSHUI_API
	#endif
#else
	#error Fengshui only supports Windows!
#endif

#ifdef FS_ENABLE_ASSERTS
	#define FS_ASSERT(x, ...) { if (!(x)) {FS_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define FS_ENGINE_ASSERT(x, ...) { if (!(x)) {FS_ENGINE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define FS_ASSERT(x, ...)
	#define FS_ENGINE_ASSERT(x, ...)
#endif

#define BIT(x) (1<<x)

#define FS_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace Fengshui
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T>
	using WeakRef = std::weak_ptr<T>;
}

#define GLM_ENABLE_EXPERIMENTAL