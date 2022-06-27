workspace "gl-playground"
    language "C++"
    cppdialect "C++20"
    architecture "x86_64"
	flags { "MultiProcessorCompile" }

    targetdir "%{wks.location}/build/%{cfg.buildcfg}"
    objdir "%{wks.location}/obj/%{cfg.buildcfg}"

    configurations {
		"Debug",
		"Release"
	}

    filter "Debug"
        defines { "DEBUG" }
        optimize "Off"
        symbols "On"

    filter "Release"
        defines { "NDEBUG" }
        optimize "On"
        symbols "Off"

	-- Indivdual Deps
	include "vendor/glfw.premake.lua"
	include "vendor/glad.premake.lua"
	include "vendor/stb_image.premake.lua"

project "gl-playground"
    kind "ConsoleApp"

	includedirs {
		"vendor/glfw/include",
		"vendor/glad/include",
		"vendor/stb_image"
	}

    files {
        "%{wks.location}/src/**.cpp",
        "%{wks.location}/src/**.h",
    }

	links {
		"glfw",
		"glad",
		"stb_image"
	}

	filter "system:linux"
		defines { "_X11" }

	filter "system:windows"
		defines { "_WINDOWS" }

	includeexternal "vendor/glfw.premake.lua"
	includeexternal "vendor/glad.premake.lua"
	includeexternal "vendor/stb_image.premake.lua"
