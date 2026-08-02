#include "ModelImporter.h"

#include "Log/Log.h"
#include "ShaderLibrary.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

aiMatrix4x4 GetMeshTransform(aiNode* node, uint32_t meshIndex, aiMatrix4x4 parentTransform = aiMatrix4x4())
{
	aiMatrix4x4 currentTransform = parentTransform * node->mTransformation;
	for (uint32_t i = 0; i < node->mNumMeshes; i++)
	{
		if (node->mMeshes[i] == meshIndex) return currentTransform;
	}
	for (uint32_t i = 0; i < node->mNumChildren; i++)
	{
		aiMatrix4x4 found = GetMeshTransform(node->mChildren[i], meshIndex, currentTransform);
		if (!found.IsIdentity()) return found;
	}
	return aiMatrix4x4();
}

glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4& from)
{
	glm::mat4 to;
	to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
	to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
	to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
	to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
	return to;
}

Model ModelImporter::LoadModel(std::filesystem::path path)
{
	Material material(ShaderLibrary::GetShader(""));
	Model retModel;
	retModel.SetSourcePath(path);

	LE_CORE_INFO("Loading model %s", path.string().c_str());

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path.string().c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals |
		aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LE_CORE_ERROR("[ASSIMP] %s", importer.GetErrorString());
		return Model();
	}


	std::vector<Mesh::Vertex> Vertices;
	std::vector<uint32_t> Indices;


	for (uint32_t i = 0; i < scene->mNumMeshes; i++)
	{
		LE_CORE_INFO("[ModelLoader] Loading mesh %s", scene->mMeshes[i]->mName.C_Str());


		for (uint32_t j = 0; j < scene->mMeshes[i]->mNumVertices; j++)
		{

			Mesh::Vertex vertex;

			vertex.Position = { scene->mMeshes[i]->mVertices[j].x,
								 scene->mMeshes[i]->mVertices[j].y,
								 scene->mMeshes[i]->mVertices[j].z };

			if (scene->mMeshes[i]->HasNormals())
			{
				vertex.Normal = { scene->mMeshes[i]->mNormals[j].x,
								  scene->mMeshes[i]->mNormals[j].y,
								  scene->mMeshes[i]->mNormals[j].z };
			}
			if (scene->mMeshes[i]->HasTangentsAndBitangents())
			{
				vertex.Tangent = { scene->mMeshes[i]->mTangents[j].x,
									 scene->mMeshes[i]->mTangents[j].y,
									 scene->mMeshes[i]->mTangents[j].z };

				vertex.Bitangent = { scene->mMeshes[i]->mBitangents[j].x,
									 scene->mMeshes[i]->mBitangents[j].y,
									 scene->mMeshes[i]->mBitangents[j].z };
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

		for (uint32_t j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
		{
			for (uint32_t k = 0; k < scene->mMeshes[i]->mFaces[j].mNumIndices; k++)
			{
				Indices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[k]);
			}
		}

		const aiMesh* model = scene->mMeshes[i];
		const aiMaterial* mtl = scene->mMaterials[model->mMaterialIndex];

		aiColor4D baseColor;
		material.BaseColor = { 255, 255, 255, 255 };
		if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &baseColor)) {
			material.BaseColor = { baseColor.r, baseColor.g, baseColor.b, baseColor.a };
		}


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
		material.SetName(model->mName.C_Str() + std::to_string(model->mMaterialIndex));

		Mesh mesh(Vertices, Indices, material);
		aiMatrix4x4 meshTransform = GetMeshTransform(scene->mRootNode, i);
		glm::mat4 mtransform = aiMatrix4x4ToGlm(meshTransform);
		mesh.SetTransform(mtransform);
		retModel.AddMesh(mesh);
	}
	return retModel;
}
