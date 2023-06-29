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

group "Dependencies" 
	include "Dependencies/imgui"
	include "Dependencies/box2d"
group ""

include "LearningEngine"
include "Sandbox"
include "LE_Editor"