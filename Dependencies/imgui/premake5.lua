include "././premake5.lua"

project "ImGui"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "Off"

    --targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    --objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	targetdir ("%{wks.location}/Dependencies/imgui/lib")
	objdir ("%{wks.location}/Dependencies/imgui/lib")
    files
    {
        "imconfig.h",
        "imgui.h",
        "imgui_internal.h",
        "imstb_rectpack.h",
        "imstb_textedit.h",
        "imstb_truetype.h",
        "imgui_draw.cpp",
        "imgui_tables.cpp",
        "imgui_widgets.cpp",
        "imgui.cpp",
        "imgui_demo.cpp",

        "misc/cpp/imgui_stdlib.h",
        "misc/cpp/imgui_stdlib.cpp"
    }

    includedirs { "%{prj.location}" }

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
