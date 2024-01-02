include "././premake5.lua"

project "Bullet"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    --targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    --objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	targetdir ("%{wks.location}/Dependencies/bullet3/lib")
	objdir ("%{wks.location}/Dependencies/bullet3/lib")
    files
    {
		"src/**.cpp",
		"src/**.h"
	}
	
	defines
	{
		"B3_USE_CLEW"
	}

    includedirs { "%{prj.location}/src" }

    filter "system:windows"
		systemversion "latest"

    filter "system:linux"
		pic "On"
		systemversion "latest"

    filter "system:macosx"
		pic "On"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"

    filter "configurations:Distribution"
        runtime "Release"
        optimize "Full"
