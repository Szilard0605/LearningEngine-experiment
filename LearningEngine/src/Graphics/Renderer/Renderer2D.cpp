#include "Renderer2D.h"

#include "gtc/matrix_transform.hpp"

#include <glew.h>

#include "Core/Base.h"

#include <ctime>

#include "Log/Log.h"
#include "ShaderLibrary.h"

//#include "API/API.h"

#include "Core/Application.h"

struct QuadVertex
{
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec2 TexCoord;
	float TexIndex;
	int Entity;
};

struct s_QuadRenderData
{
	const uint32_t MaxQuads = 20000;
	const uint32_t MaxVertices = MaxQuads * 4;
	const uint32_t MaxIndices = MaxQuads * 6;
	const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

	uint32_t QuadCount;

	VertexArray* vertexarray;
	VertexBuffer* vertexbuffer;
	IndexBuffer* indexbuffer;

	glm::mat4 transform = glm::mat4(1.0);

	Shader* FlatColorShader;
	Shader* TextureShader;

	uint32_t QuadIndexCount = 0;

	QuadVertex* QuadVertexBufferPtr = nullptr;
	QuadVertex* QuadVertexBufferBase = nullptr;

	glm::vec4 QuadVertexPositions[4];

	//GLTexture2D* texture_slot = new GLTexture2D[MaxTextureSlots];

	Texture2D* WhiteTexture;
	std::vector<Texture2D*> Textures;
	uint32_t TextureSlotIndex = 1;

	//GLTexture2D* texture;
};

static s_QuadRenderData g_RenderData;

struct RenderStatistics
{
	uint32_t DrawCalls = 0;
	uint32_t RenderTimeOnCPU = 0;
};

static RenderStatistics s_RenderStats;

void Renderer2D::Init()
{
	g_RenderData.WhiteTexture = Texture2D::Create(1, 1);
	uint32_t whiteTextureData = 0xffffffff;
	g_RenderData.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

	for (uint32_t i = 0; i < g_RenderData.MaxTextureSlots; i++)
		g_RenderData.Textures.push_back(g_RenderData.WhiteTexture);


	g_RenderData.FlatColorShader = ShaderLibrary::Load("Default_FlatColor", "res/shaders/def_shader.shader");
	g_RenderData.TextureShader = ShaderLibrary::Load("Default_Texture", "res/shaders/texture_shader.shader");

	g_RenderData.vertexbuffer = VertexBuffer::Create(g_RenderData.MaxVertices * sizeof(QuadVertex));


	g_RenderData.vertexbuffer->SetLayout({
		{ ShaderDataType::Float3, "a_position" },
		{ ShaderDataType::Float4, "a_color" },
		{ ShaderDataType::Float2, "a_texcoord" },
		{ ShaderDataType::Float, "a_texindex" },
		{ ShaderDataType::Int,  "a_entity"	},
	});

	g_RenderData.vertexarray = VertexArray::Create();
	g_RenderData.vertexarray->AddVertexBuffer(g_RenderData.vertexbuffer);

	uint32_t* quadIndices = new uint32_t[g_RenderData.MaxIndices];

	uint32_t offset = 0;
	for (uint32_t i = 0; i < g_RenderData.MaxIndices; i += 6)
	{
		quadIndices[i + 0] = offset + 0;
		quadIndices[i + 1] = offset + 1;
		quadIndices[i + 2] = offset + 2;

		quadIndices[i + 3] = offset + 2;
		quadIndices[i + 4] = offset + 3;
		quadIndices[i + 5] = offset + 0;

		offset += 4;
	}

	// set index buffer
	g_RenderData.indexbuffer = IndexBuffer::Create(quadIndices, g_RenderData.MaxIndices);
	//g_RenderData.vertexarray->SetIndexBuffer(*indexbuffer);

	delete[] quadIndices;

	g_RenderData.QuadVertexBufferBase = new QuadVertex[g_RenderData.MaxVertices];

	g_RenderData.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
	g_RenderData.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
	g_RenderData.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
	g_RenderData.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };


}

void Renderer2D::Begin(OrthographicCamera& Camera)
{
	Camera.CalculateViewMatrix();

	g_RenderData.TextureShader->SetMatrix4f("u_ViewProjection", Camera.GetViewProjection());
	g_RenderData.FlatColorShader->SetMatrix4f("u_ViewProjection", Camera.GetViewProjection());

	StartBatch();
}

void Renderer2D::Begin(PerspectiveCamera& Camera)
{
	Camera.UpdateView();

	g_RenderData.TextureShader->SetMatrix4f("u_ViewProjection", Camera.GetViewProjection());
	g_RenderData.FlatColorShader->SetMatrix4f("u_ViewProjection", Camera.GetViewProjection());

	StartBatch();
}


void Renderer2D::End()
{
	Flush();
}

void Renderer2D::ClearColor(const glm::vec4& color)
{
	//GLRenderer::ClearColor(color.r, color.g, color.b, color.a);
	Application::GetInstance()->GetRenderer()->Clear(color);
	
}

void Renderer2D::Clear()
{
	//Application::GetInstance()->GetRenderer()->Clear(color.r, color.g, color.b, color.a);
}

void Renderer2D::NextBatch()
{
	StartBatch();
	Flush();
}

void Renderer2D::StartBatch()
{
	g_RenderData.QuadIndexCount = 0;
	g_RenderData.QuadCount = 0;
	g_RenderData.QuadVertexBufferPtr = g_RenderData.QuadVertexBufferBase;
	g_RenderData.QuadVertexBufferPtr->Entity = -1;
	g_RenderData.TextureSlotIndex = 1;

	s_RenderStats.DrawCalls++;

}

void Renderer2D::Flush()
{
	if (g_RenderData.QuadCount)
	{
		uint32_t dataSize = (uint32_t)((uint8_t*)g_RenderData.QuadVertexBufferPtr - (uint8_t*)g_RenderData.QuadVertexBufferBase);
		g_RenderData.vertexbuffer->SetData(g_RenderData.QuadVertexBufferBase, dataSize);

		for (uint32_t i = 0; i < g_RenderData.MaxTextureSlots; i++)
			g_RenderData.Textures[i]->Bind(i);

		g_RenderData.TextureShader->Bind();
		g_RenderData.vertexarray->Bind();
		g_RenderData.indexbuffer->Bind();
		g_RenderData.vertexarray->DrawIndexed(g_RenderData.QuadIndexCount);

		s_RenderStats.DrawCalls++;
	}
}



void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec3& scale, const float rotation, const glm::vec4& color, int entity)
{
	constexpr size_t quadVertexCount = 4;
	const float textureIndex = 0.0f; // White Texture
	constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
	const float tilingFactor = 1.0f;

	const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
		* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
		* glm::scale(glm::mat4(1.0f), scale);

	for (size_t i = 0; i < quadVertexCount; i++)
	{
		g_RenderData.QuadVertexBufferPtr->Position = transform * g_RenderData.QuadVertexPositions[i];
		g_RenderData.QuadVertexBufferPtr->Color = color;
		g_RenderData.QuadVertexBufferPtr->TexCoord = textureCoords[i];
		g_RenderData.QuadVertexBufferPtr->TexIndex = textureIndex;
		g_RenderData.QuadVertexBufferPtr->Entity = entity;

		g_RenderData.QuadVertexBufferPtr++;
	}

	g_RenderData.QuadIndexCount += 6;
	g_RenderData.QuadCount++;


}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec3& scale, const float rotation, const glm::vec4& color, Texture2D* texture)
{
	constexpr size_t quadVertexCount = 4;
	constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
	const float tilingFactor = 1.0f;

	const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
		* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
		* glm::scale(glm::mat4(1.0f), scale);

	//g_QuadRenderData.shader->SetMatrix4f("u_Transform", transform);


	float textureIndex = 0.0f;
	for (uint32_t i = 1; i < g_RenderData.TextureSlotIndex; i++)
	{
		if (g_RenderData.Textures[i] == texture)
		{
			textureIndex = (float)i;
			break;
		}
	}

	if (textureIndex == 0.0f)
	{
		if (g_RenderData.TextureSlotIndex >= g_RenderData.MaxTextureSlots)
			NextBatch();

		textureIndex = (float)g_RenderData.TextureSlotIndex;
		g_RenderData.Textures[g_RenderData.TextureSlotIndex] = texture;
		g_RenderData.TextureSlotIndex++;
	}

	for (size_t i = 0; i < quadVertexCount; i++)
	{
		g_RenderData.QuadVertexBufferPtr->Position = transform * g_RenderData.QuadVertexPositions[i];
		g_RenderData.QuadVertexBufferPtr->Color = color;
		g_RenderData.QuadVertexBufferPtr->TexCoord = textureCoords[i];
		g_RenderData.QuadVertexBufferPtr->TexIndex = textureIndex;

		g_RenderData.QuadVertexBufferPtr++;
	}

	g_RenderData.QuadIndexCount += 6;
	g_RenderData.QuadCount++;

	//g_RenderData.Textures[g_RenderData.QuadCount] = &texture;
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation, const glm::vec4& color, int entity)
{
	constexpr size_t quadVertexCount = 4;
	const float textureIndex = 0.0f; // White Texture
	constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
	const float tilingFactor = 1.0f;

	const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
		* glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), { 1.0f, 0.0f, 0.0f })
		* glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), { 0.0f, 1.0f, 0.0f })
		* glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), { 0.0f, 0.0f, 1.0f })
		* glm::scale(glm::mat4(1.0f), scale);

	for (size_t i = 0; i < quadVertexCount; i++)
	{
		g_RenderData.QuadVertexBufferPtr->Position = transform * g_RenderData.QuadVertexPositions[i];
		g_RenderData.QuadVertexBufferPtr->Color = color;
		g_RenderData.QuadVertexBufferPtr->TexCoord = textureCoords[i];
		g_RenderData.QuadVertexBufferPtr->TexIndex = textureIndex;
		g_RenderData.QuadVertexBufferPtr->Entity = entity;

		g_RenderData.QuadVertexBufferPtr++;
	}

	g_RenderData.QuadIndexCount += 6;
	g_RenderData.QuadCount++;
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation, const glm::vec4& color, Texture2D* texture)
{
	constexpr size_t quadVertexCount = 4;
	constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
	const float tilingFactor = 1.0f;

	const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
		* glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), { 1.0f, 0.0f, 0.0f })
		* glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), { 0.0f, 1.0f, 0.0f })
		* glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), { 0.0f, 0.0f, 1.0f })
		* glm::scale(glm::mat4(1.0f), scale);

	//g_QuadRenderData.shader->SetMatrix4f("u_Transform", transform);


	float textureIndex = 0.0f;
	for (uint32_t i = 1; i < g_RenderData.TextureSlotIndex; i++)
	{
		if (g_RenderData.Textures[i] == texture)
		{
			textureIndex = (float)i;
			break;
		}
	}

	if (textureIndex == 0.0f)
	{
		if (g_RenderData.TextureSlotIndex >= g_RenderData.MaxTextureSlots)
			NextBatch();

		textureIndex = (float)g_RenderData.TextureSlotIndex;
		g_RenderData.Textures[g_RenderData.TextureSlotIndex] = texture;
		g_RenderData.TextureSlotIndex++;
	}

	for (size_t i = 0; i < quadVertexCount; i++)
	{
		g_RenderData.QuadVertexBufferPtr->Position = transform * g_RenderData.QuadVertexPositions[i];
		g_RenderData.QuadVertexBufferPtr->Color = color;
		g_RenderData.QuadVertexBufferPtr->TexCoord = textureCoords[i];
		g_RenderData.QuadVertexBufferPtr->TexIndex = textureIndex;

		g_RenderData.QuadVertexBufferPtr++;
	}

	g_RenderData.QuadIndexCount += 6;
	g_RenderData.QuadCount++;

}

/*void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation, const glm::vec4& color, GLSubTexture2D& texture)
{
	constexpr size_t quadVertexCount = 4;

	glm::vec2 SpritePos = texture.GetPosition();
	glm::vec2 SpriteSize = texture.GetCellSize();
	glm::vec2 SpriteSheetSize = { texture.GetParentTexture()->width, texture.GetParentTexture()->height };
	const float tilingFactor = 1.0f;
	glm::vec2* textureCoords = texture.GetTextureCoords();

	const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
		* glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), { 1.0f, 0.0f, 0.0f })
		* glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), { 0.0f, 1.0f, 0.0f })
		* glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), { 0.0f, 0.0f, 1.0f })
		* glm::scale(glm::mat4(1.0f), scale);

	//g_QuadRenderData.shader->SetMatrix4f("u_Transform", transform);


	float textureIndex = 0.0f;
	for (uint32_t i = 1; i < g_RenderData.TextureSlotIndex; i++)
	{
		if (g_RenderData.Textures[i] == texture.GetParentTexture())
		{
			textureIndex = (float)i;
			break;
		}
	}

	if (textureIndex == 0.0f)
	{
		if (g_RenderData.TextureSlotIndex >= g_RenderData.MaxTextureSlots)
			NextBatch();

		textureIndex = (float)g_RenderData.TextureSlotIndex;
		g_RenderData.Textures[g_RenderData.TextureSlotIndex] = texture.GetParentTexture();
		g_RenderData.TextureSlotIndex++;
	}

	for (size_t i = 0; i < quadVertexCount; i++)
	{
		g_RenderData.QuadVertexBufferPtr->Position = transform * g_RenderData.QuadVertexPositions[i];
		g_RenderData.QuadVertexBufferPtr->Color = color;
		g_RenderData.QuadVertexBufferPtr->TexCoord = textureCoords[i];
		g_RenderData.QuadVertexBufferPtr->TexIndex = textureIndex;

		g_RenderData.QuadVertexBufferPtr++;
	}

	g_RenderData.QuadIndexCount += 6;
	g_RenderData.QuadCount++;

}*/

