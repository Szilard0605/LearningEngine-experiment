include "./premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "LearningEngine"
	architecture "x64"
    	startproject "Sandbox"

	configurations { "Debug", "Release", "Dist" }
	
	flags 
    	{ 
		"MultiProcessorCompile" 
    	}

    	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Applications"
include "Sandbox"
include "LE_Editor"
include "LE_Launcher"
group ""

group "Core"
include "LearningEngine"
group ""

group "Dependencies" 
	include "Dependencies/imgui"
	include "Dependencies/box2d"
	include "Dependencies/assimp"
group ""


