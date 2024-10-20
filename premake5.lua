workspace "GameEngine"
	architecture "x64"
	startproject "Fengshui-Editor"
	-- startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

output_dir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Fengshui/External/GLFW/include"
IncludeDir["GLAD"] = "Fengshui/External/GLAD/include"
IncludeDir["IMGUI"] = "Fengshui/External/IMGUI"
IncludeDir["GLM"] = "Fengshui/External/GLM"
IncludeDir["STB"] = "Fengshui/External/stb_image"
IncludeDir["OPENAL"] = "Fengshui/External/Openal-Soft/include"
IncludeDir["MINIAUDIO"] = "Fengshui/External/miniaudio"

group "Dependencies"
	include "Fengshui/External/GLFW"
	include "Fengshui/External/GLAD"
	include "Fengshui/External/IMGUI"
	include "Fengshui/External/Openal-Soft"
group ""

project "Fengshui"
	location "Fengshui"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. output_dir .. "/%{prj.name}")
	objdir ("bin-int/" .. output_dir .. "/%{prj.name}")

	pchheader "fspch.h"
	pchsource "Fengshui/src/fspch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/External/stb_image/**.h",
		"%{prj.name}/External/stb_image/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/External/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.IMGUI}",
		"%{IncludeDir.IMGUI}/backends",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.STB}",
		"%{IncludeDir.OPENAL}",
		"Fengshui/External/Openal-Soft/src",
		"Fengshui/External/OpenAL-Soft/src/common",
		"%{IncludeDir.MINIAUDIO}",
	}
	
	links
	{
		"GLFW",
		"GLAD",
		"IMGUI",
		"OpenAL-Soft",
	}

	defines
	{
		"GLFW_INCLUDE_NONE",
		"_CRT_SECURE_NO_WARNINGS",
		"AL_LIBTYPE_STATIC",
		"MINIAUDIO_IMPLEMENTATION",
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"FS_BUILD_DLL",
			"FS_PLATFORM_WINDOWS",
		}

		filter "configurations:Debug"
			symbols "on"
			runtime "Debug"

			defines
			{
				"FS_DEBUG",
				"FS_ENABLE_ASSERTS",
			}

		filter "configurations:Release"
			optimize "on"
			runtime "Release"

			defines{
				"FS_RELEASE",
			}

		filter "configurations:Dist"
			optimize "on"
			runtime "Release"

			defines{
				"FS_DIST",
			}

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. output_dir .. "/%{prj.name}")
	objdir ("bin-int/" .. output_dir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Fengshui/External/spdlog/include",
		"Fengshui/src",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.IMGUI}",
	}

	links
	{
		"Fengshui"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"FS_PLATFORM_WINDOWS"
		}

		filter "configurations:Debug"
			defines "FS_DEBUG"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			defines "FS_RELEASE"
			runtime "Release"
			optimize "on"

		filter "configurations:Dist"
			defines "FS_DIST"
			runtime "Release"
			optimize "on"

project "Fengshui-Editor"
	location "Fengshui-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. output_dir .. "/%{prj.name}")
	objdir ("bin-int/" .. output_dir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Fengshui/External/spdlog/include",
		"Fengshui/src",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.IMGUI}",
	}

	links
	{
		"Fengshui",
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"FS_PLATFORM_WINDOWS"
		}

		filter "configurations:Debug"
			defines "FS_DEBUG"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			defines "FS_RELEASE"
			runtime "Release"
			optimize "on"

		filter "configurations:Dist"
			defines "FS_DIST"
			runtime "Release"
			optimize "on"