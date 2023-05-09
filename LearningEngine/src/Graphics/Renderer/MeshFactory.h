#pragma once
#include "Mesh.h"

#include "Graphics/Camera/PerspectiveCamera.h"


class MeshFactory
{
public:
	static Mesh* CreateCube(float size, Material& material);

private:
	MeshFactory() {}
	~MeshFactory() {}
};
