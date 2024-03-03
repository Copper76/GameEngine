workspace "GameEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

output_dir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["SFML"] = "Fengshui/External/SFML/include"
IncludeDir["GLFW"] = "Fengshui/External/GLFW/include"

include "Fengshui/External/GLFW"

project "Fengshui"
	location "Fengshui"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
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
		"%{IncludeDir.SFML}",
		"%{IncludeDir.GLFW}"
	}

	libdirs
	{
		"%{prj.name}/External/SFML/lib"
	}

	links
	{
		"GLFW",
		"opengl32",  
		"freetype",
		"winmm",
		"gdi32"
	}

	filter "system:windows"
		staticruntime "Off"
		systemversion "latest"

		defines
		{
			"FS_BUILD_DLL",
			"SFML_STATIC",
			"FS_PLATFORM_WINDOWS",
			"_CRT_SECURE_NO_WARNINGS"
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
			}

			links
			{
				"sfml-graphics-s-d",
				"sfml-window-s-d",
				"sfml-system-s-d",
			}

		filter "configurations:RELEASE"
			optimize "On"

			defines{
				"FS_RELEASE",
			}

			links
			{
				"sfml-graphics-s",
				"sfml-window-s",
				"sfml-system-s",
			}

		filter "configurations:DIST"
			optimize "On"

			defines{
				"FS_DIST",
			}

			links
			{
				"sfml-graphics-s",
				"sfml-window-s",
				"sfml-system-s",
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
		"Fengshui/src",
		"Fengshui/External/spdlog/include"
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