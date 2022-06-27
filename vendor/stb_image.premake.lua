project "stb_image"
	kind "StaticLib"
	language "C"
	staticruntime "On"
	architecture "x86_64"

	targetdir "../build/%{prj.name}"
	objdir "../obj/%{prj.name}"

	files {
		"stb_image/stb_image.h",
		"stb_image/stb_image.cpp"
	}

	filter "system:linux"
		pic "On"
		systemversion "latest"

		defines {
			"_STB_IMAGE_X11",
		}

	filter "system:windows"
		systemversion "latest"

		defines {
			"_STB_IMAGE_WIN32",
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
