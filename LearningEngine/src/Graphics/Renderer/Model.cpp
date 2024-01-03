#include "Model.h"

#include "Log/Log.h"
#include "Graphics/Renderer/ShaderLibrary.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "API/API.h"

Model::Model(std::filesystem::path path) : Model(path, Material(ShaderLibrary::GetShader("DefaultShader")))
{
}

Model::Model(std::filesystem::path path, Material material)
	: m_Path(path)
{
	LE_CORE_INFO("Loading model %s", path.string().c_str());

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path.string().c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals |
		aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LE_CORE_ERROR("[ASSIMP] %s", importer.GetErrorString());
		return;
	}

	for (uint32_t i = 0; i < scene->mNumMeshes; i++)
	{
		std::vector<Mesh::Vertex> Vertices;
		std::vector<uint32_t> Indices;

		for (uint32_t j = 0; j < scene->mMeshes[i]->mNumVertices; j++)
		{

			Mesh::Vertex vertex;


			vertex.Position =  { scene->mMeshes[i]->mVertices[j].x,
								 scene->mMeshes[i]->mVertices[j].y,
								 scene->mMeshes[i]->mVertices[j].z   };
			
			vertex.Normal =	   { scene->mMeshes[i]->mNormals[j].x,
								 scene->mMeshes[i]->mNormals[j].y,
								 scene->mMeshes[i]->mNormals[j].z    };
			
			if (scene->mMeshes[i]->HasTangentsAndBitangents())
			{
				vertex.Tangent = {   scene->mMeshes[i]->mTangents[j].x,
									 scene->mMeshes[i]->mTangents[j].y,
									 scene->mMeshes[i]->mTangents[j].z};

				vertex.Bitangent = { scene->mMeshes[i]->mBitangents[j].x,
									 scene->mMeshes[i]->mBitangents[j].y,
									 scene->mMeshes[i]->mBitangents[j].z};
			}

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

		Texture2D* normalMapTexture = nullptr;
		// Normal Map textures
		{
			aiString texpath;	// filename
			aiReturn texFound = mtl->GetTexture(aiTextureType_NORMALS, 0, &texpath);

			if (texFound == AI_SUCCESS)
			{
				normalMapTexture = Texture2D::Create(path.remove_filename().string() + texpath.C_Str());
			}
		}
		material.SetNormalMap(normalMapTexture);


		m_Meshes.push_back(Mesh(Vertices, Indices, material));
	}

}

void Model::Render(PerspectiveCamera& camera, glm::mat4 transform)
{
	for(int i = 0; i < m_Meshes.size(); i ++)
	{
		m_Meshes[i].Render(camera, transform);
	}
}

void Model::AddMesh(Mesh mesh)
{
	m_Meshes.push_back(mesh);
}
