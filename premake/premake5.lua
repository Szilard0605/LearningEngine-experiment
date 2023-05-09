project "Premake"
	kind "Utility"

	targetdir ("%{wks.location}/bin/" .. outputdir")
	objdir ("%{wks.location}/bin-int/" .. outputdir")

	files
	{
		"%{wks.location}/**premake5.lua"
	}

	postbuildmessage "Regenerating project files with Premake5!"
	postbuildcommands
	{
		"\"%{prj.location}bin/premake5\" %{_ACTION} --file=\"%{wks.location}premake5.lua\""
	}