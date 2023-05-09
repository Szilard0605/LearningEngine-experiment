project "LE_Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir)
	objdir ("%{wks.location}/bin-int/" .. outputdir)
	debugdir ("%{wks.location}/bin/" .. outputdir)

	files
	{
		"**.h",
		"**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Dependencies/spdlog/include",
		"%{wks.location}/LearningEngine",
		"%{wks.location}/LearningEngine/src",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLEW_STATIC"
	}

	links
	{
		"LearningEngine"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "HZ_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "LE_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "LE_DIST"
		runtime "Release"
		optimize "on"
