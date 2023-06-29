#include "Model.h"

#include "Log/Log.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "API/API.h"

Model::Model(std::filesystem::path path, Material& material)
	: m_Path(path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path.string().c_str(), aiProcess_Triangulate);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LE_LOG_ERROR("[ASSIMP] " + std::string(importer.GetErrorString()));
		return;
	}

	for (uint32_t i = 0; i < scene->mNumMeshes; i++)
	{
		std::vector<Vertex> Vertices;
		std::vector<uint32_t> Indices;

		for (uint32_t j = 0; j < scene->mMeshes[i]->mNumVertices; j++)
		{
			const float vertex_X = scene->mMeshes[i]->mVertices[j].x;
			const float vertex_Y = scene->mMeshes[i]->mVertices[j].y;
			const float vertex_Z = scene->mMeshes[i]->mVertices[j].z;

			const float normal_X = scene->mMeshes[i]->mVertices[j].x;
			const float normal_Y = scene->mMeshes[i]->mVertices[j].y;
			const float normal_Z = scene->mMeshes[i]->mVertices[j].z;

			Vertex vertex;

			if (scene->mMeshes[i]->mTextureCoords[0]) 
			{
				const float uv_X = scene->mMeshes[i]->mTextureCoords[0][j].x;
				const float uv_Y = scene->mMeshes[i]->mTextureCoords[0][j].y;
				vertex.TexCoords = glm::vec2(uv_X, uv_Y);
			}
			else
			{
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			}

			vertex.Position = glm::vec3(vertex_X, vertex_Y, vertex_Z);
			vertex.Normal = glm::vec3(normal_X, normal_Y, normal_Z);
			Vertices.push_back(vertex);
		}


		Indices.reserve((size_t)scene->mMeshes[i]->mNumFaces * 3);

		for (uint32_t j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
		{
			for (uint32_t k = 0; k < scene->mMeshes[i]->mFaces[j].mNumIndices; k++)
			{
				Indices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[k]);
			}
		}

		const aiMesh* model = scene->mMeshes[i];
		const aiMaterial* mtl = scene->mMaterials[model->mMaterialIndex];

		Texture2D* baseColorTexture = nullptr;
		// Base Color textures
		{
			aiString texpath;	// filename
			aiReturn texFound = mtl->GetTexture(aiTextureType_DIFFUSE, 0, &texpath);

			if (texFound == AI_SUCCESS)
			{
				baseColorTexture = Texture2D::Create(path.remove_filename().string() + texpath.C_Str());
			}
		}
		material.SetTexture(baseColorTexture);


		m_Meshes.push_back(new Mesh(Vertices, Indices, material));
	}

}

void Model::Render(PerspectiveCamera& camera, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
{
	for(int i = 0; i < m_Meshes.size(); i ++)
	{
		m_Meshes[i]->Render(camera, position, scale, rotation);
	}
}
