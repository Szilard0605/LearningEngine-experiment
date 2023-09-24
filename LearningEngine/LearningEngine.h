#pragma once


/*
* 
* Game Engine, based on OpenGL,
* made for learning purposes.
* Made by: Kátai Szilárd
* 
*/

#include "Core/Application.h"
#include "Core/Input.h"
#include "Core/Timestep.h"

#include "Log/Log.h"

#include "Graphics/Renderer/API/API.h"

#include "Graphics/Renderer/Renderer2D.h"
#include "Graphics/Renderer/ShaderLibrary.h"
#include "Graphics/Renderer/Model.h"
#include "Graphics/Renderer/Mesh.h"
#include "Graphics/Renderer/MeshFactory.h"
#include "Graphics/Renderer/Material.h"
#include "Graphics/Renderer/ForwardRenderer.h"

#include "Graphics/Scene/Scene.h"
#include "Graphics/Scene/Entity.h"
#include "Graphics/Scene/Components.h"
#include "Graphics/Scene/SceneSerializer.h"

#include "Graphics/Camera/OrthographicCamera.h"
#include "Graphics/Camera/PerspectiveCamera.h"

#include "Assets/Asset.h"
#include "Assets/AssetManager.h"
#include "Assets/AssetRegistry.h"
#include "Assets/AssetLoader.h"

#include "Events/Events.h"

#include "Utils/FileDialog.h"
