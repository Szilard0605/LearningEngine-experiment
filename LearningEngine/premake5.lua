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
		"ImGui",
		"Bullet"
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

		"./Dependencies/tinyxml2/**.h",
		"./Dependencies/tinyxml2/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLEW_STATIC",
		"NOMINMAX"
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
		"%{IncludeDir.ASSIMP}",
		"%{IncludeDir.ENTT}",
		"%{IncludeDir.JSON}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.Bullet}"
	}
	
	libdirs
	{
		"%{LibraryDir.GLEW}",
		"%{LibraryDir.GLFW}",
		"%{LibraryDir.ImGui}",
		"%{LibraryDir.ASSIMP64}"
	}

	links
	{
		"Bullet",
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
			"ImGui",
			"glew64s.lib",
			"glfw3_64.lib",
			"opengl32.lib"
		}