project "LearningEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir)
	objdir ("%{wks.location}/bin-int/" .. outputdir)

	files
	{
		"**.h",
		"**.cpp",
		"src/**.h",
		"src/**.cpp",
		"src/utils/stb_image/**.h",
		"src/utils/stb_image/**.cpp",
		"./Dependencies/glm/include/**.hpp",
		"./Dependencies/glm/include/**.inl",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLEW_STATIC"
	}

	includedirs
	{
		"src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.GLEW}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.ASSIMP}",
		"%{IncludeDir.ENTT}",
		"%{IncludeDir.JSON}"		
	}
	
	libdirs
	{
		"%{LibraryDir.GLEW}",
		"%{LibraryDir.GLFW}",
		"%{LibraryDir.ImGui}",
		"%{LibraryDir.Box2D}",
		"%{LibraryDir.ASSIMP}"
	}

	links
	{
		"Box2D",
		"ImGui",
		"glew32s.lib",
		"glfw3.lib",
		"opengl32.lib",
		"assimp.lib"
	}

	flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"

		defines
		{
		}

	filter "configurations:Debug"
		defines "LE_DEBUG"
		runtime "Debug"
		symbols "on"

		links
		{
			"Box2D",
			"ImGui",
			"glew32s.lib",
			"glfw3.lib",
			"opengl32.lib"
		}

	filter "configurations:Release"
		defines "LE_RELEASE"
		runtime "Release"
		optimize "on"

		links
		{
			"Box2D",
			"ImGui",
			"glew32s.lib",
			"glfw3.lib",
			"opengl32.lib"
		}