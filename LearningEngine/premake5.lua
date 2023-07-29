project "LearningEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir)
	objdir ("%{wks.location}/bin-int/" .. outputdir)

	dependson 
	{
		"Assimp",
		"Box2D",
		"ImGui"
	}

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

		"../Dependencies/ImGuizmo/ImGuizmo.h",
		"../Dependencies/ImGuizmo/ImGuizmo.cpp"
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
		"%{IncludeDir.JSON}",
		"%{IncludeDir.ImGuizmo}"
	}
	
	libdirs
	{
		"%{LibraryDir.GLEW}",
		"%{LibraryDir.GLFW}",
		"%{LibraryDir.ImGui}",
		"%{LibraryDir.Box2D}",
		"%{LibraryDir.ASSIMP64}"
	}

	links
	{
		"Box2D",
		"ImGui",
		"glew64s.lib",
		"glfw3_64.lib",
		"opengl32.lib",
		"Assimp.lib"
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
			"glew64s.lib",
			"glfw3_64.lib",
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
			"glew64s.lib",
			"glfw3_64.lib",
			"opengl32.lib"
		}