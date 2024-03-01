workspace "GameEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

output_dir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Fengshui"
	location "Fengshui"
	kind "SharedLib"
	language "C++"
	targetdir ("bin/" .. output_dir .. "/%{prj.name}")
	objdir ("bin-int/" .. output_dir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/External/SFML/include"
	}

	libdirs
	{
		"%{prj.name}/External/SFML/lib"
	}

	links
	{
		"opengl32",  
		"freetype",
		"winmm",
		"gdi32"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "Off"
		systemversion "latest"

		defines
		{
			"FS_BUILD_DLL",
			"SFML_STATIC",
			"FS_PLATFORM_WINDOWS"
		}

		postbuildcommands
		{
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
			defines{
				"FS_DIST",
			}

			optimize "On"

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
	targetdir ("bin/" .. output_dir .. "/%{prj.name}")
	objdir ("bin-int/" .. output_dir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Fengshui/src"
	}

	links
	{
		"Fengshui"
	}

	filter "system:windows"
		cppdialect "C++20"
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