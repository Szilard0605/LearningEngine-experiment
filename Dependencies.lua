IncludeDir = {}

IncludeDir["GLFW"] = "%{wks.location}/Dependencies/glfw/include"
IncludeDir["ImGui"] = "%{wks.location}/Dependencies/imgui"
IncludeDir["glm"] = "%{wks.location}/Dependencies/glm/include"
IncludeDir["GLEW"] = "%{wks.location}/Dependencies/glew/include"
IncludeDir["spdlog"] = "%{wks.location}/Dependencies/spdlog/include"
IncludeDir["Box2D"] = "%{wks.location}/Dependencies/box2d/include"
IncludeDir["ASSIMP"] = "%{wks.location}/Dependencies/assimp/include"
IncludeDir["ENTT"] = "%{wks.location}/Dependencies/entt/include"
IncludeDir["JSON"] = "%{wks.location}/Dependencies/json"

LibraryDir = {}

LibraryDir["GLEW"] = "%{wks.location}/Dependencies/glew/lib"
LibraryDir["GLFW"] = "%{wks.location}/Dependencies/glfw/lib"
LibraryDir["ImGui"] = "%{wks.location}/Dependencies/imgui/lib"
LibraryDir["Box2D"] = "%{wks.location}/Dependencies/box2d/lib"
LibraryDir["ASSIMP"] = "%{wks.location}/Dependencies/assimp/bin"
LibraryDir["ASSIMP64"] = "%{wks.location}/Dependencies/assimp/bin"