#pragma once

#include "Core/Base.h"
#include "Graphics/Camera/OrthographicCamera.h"
#include "Graphics/Camera/PerspectiveCamera.h"

#include "API/API.h"

class Renderer2D
{
public:
	static void Init();
	static void Begin(OrthographicCamera& Camera);
	static void Begin(PerspectiveCamera& Camera);
	static void End();
	static void Clear();
	static void NextBatch();
	static void StartBatch();
	static void Flush();
	static void ClearColor(const glm::vec4& color);


	static void DrawQuad(const glm::vec3& position, const glm::vec3& scale, const float rotation, const glm::vec4& color, int entity = -1);
	static void DrawQuad(const glm::vec3& position, const glm::vec3& scale, const float rotation, const glm::vec4& color, Texture2D* texture);
									   						  	  
	static void DrawQuad(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation, const glm::vec4& color, int entity = -1);
	static void DrawQuad(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation, const glm::vec4& color, Texture2D* texture);
									   						  	  
	//static void DrawQuad(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation, const glm::vec4& color, GLSubTexture2D& texture);
private:
	Renderer2D() {};
	~Renderer2D() {};

};

