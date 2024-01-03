include "././premake5.lua"

project "Lua"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

	targetdir ("%{wks.location}/Dependencies/lua/lib")
	objdir ("%{wks.location}/Dependencies/lua/lib/obj")
	
    files
    {
		"**.c",
        "**.h"
    }

    includedirs { "%{prj.location}" }

    filter "system:windows"
		systemversion "latest"

    filter "system:linux"
		pic "On"
		systemversion "latest"

    filter "system:macosx"
		pic "On"

    filter  "configurations:Debug"
		staticruntime "on"
		runtime "Debug"
		symbols "on"

   filter  "configurations:Release"
		staticruntime "on"
		runtime "Release"
		optimize "on"

    filter "configurations:Distribution"
        runtime "Release"
        optimize "Full"
