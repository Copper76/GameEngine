workspace "GameEngine"
	architecture "x64"
	startproject "Sandbox"

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
IncludeDir["imgui"] = "Fengshui/External/imgui"
IncludeDir["GLM"] = "Fengshui/External/GLM"

include "Fengshui/External/GLFW"
include "Fengshui/External/GLAD"
include "Fengshui/External/imgui"

project "Fengshui"
	location "Fengshui"
	kind "SharedLib"
	language "C++"
	targetdir ("bin/" .. output_dir .. "/%{prj.name}")
	objdir ("bin-int/" .. output_dir .. "/%{prj.name}")

	pchheader "fspch.h"
	pchsource "Fengshui/src/fspch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/External/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.imgui}/backends",
		"%{IncludeDir.GLM}",
	}

	links
	{
		"GLFW",
		"GLAD",
		"imgui",
		"opengl32",
	}

	defines
	{
		"GLFW_INCLUDE_NONE",
	}

	filter "system:windows"
		staticruntime "Off"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"FS_BUILD_DLL",
			"FS_PLATFORM_WINDOWS",
			"_CRT_SECURE_NO_WARNINGS",
		}

		postbuildcommands
		{
			("if not exist \"../bin/" .. output_dir .. "/Sandbox/\" mkdir \"../bin/" .. output_dir .. "/Sandbox/\""),
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. output_dir .. "/Sandbox")
		}

		filter "configurations:Debug"
			symbols "On"

			defines
			{
				"FS_DEBUG",
				"FS_ENABLE_ASSERTS",
			}

		filter "configurations:RELEASE"
			optimize "On"

			defines{
				"FS_RELEASE",
			}

		filter "configurations:DIST"
			optimize "On"

			defines{
				"FS_DIST",
			}

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
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
	}

	links
	{
		"Fengshui"
	}

	filter "system:windows"
		staticruntime "Off"
		systemversion "latest"

		defines
		{
			"FS_PLATFORM_WINDOWS"
		}

		filter "configurations:Debug"
			defines "FS_DEBUG"
			optimize "On"

		filter "configurations:RELEASE"
			defines "FS_RELEASE"
			symbols "On"

		filter "configurations:DIST"
			defines "FS_DIST"
			optimize "On"