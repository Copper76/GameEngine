#pragma once

#include <memory>

//Several Macro definitions to help with development
#ifdef FS_ENABLE_ASSERTS
	#define FS_ASSERT(x, ...) { if (!(x)) {FS_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define FS_ENGINE_ASSERT(x, ...) { if (!(x)) {FS_ENGINE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define FS_ASSERT(x, ...)
	#define FS_ENGINE_ASSERT(x, ...)
#endif

#define BIT(x) (1<<x)

#define FS_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

//Defined to make the naming more concise
namespace Fengshui
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename... Args>
	std::shared_ptr<T> MakeRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using WeakRef = std::weak_ptr<T>;
}

//Defined to use the length2 method in glm
#define GLM_ENABLE_EXPERIMENTAL