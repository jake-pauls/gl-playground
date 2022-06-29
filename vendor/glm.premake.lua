project "glm"
	kind "StaticLib"
	language "C"
	staticruntime "On"
	architecture "x86_64"

	targetdir "../build/%{prj.name}"
	objdir "../obj/%{prj.name}"

	includedirs { "glm/" }
	files { "glm/glm/**" }

	defines {
		"GLM_FORCE_SWIZZLE"
	}

	filter "system:linux"
		pic "On"
		systemversion "latest"

		defines {
			"_GLM_X11"
		}

	filter "system:windows"
		systemversion "latest"

		defines {
			"_GLM_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		optimize "Off"

	filter "configurations:Release"
		runtime "Release"
		symbols "Off"
		optimize "On"
