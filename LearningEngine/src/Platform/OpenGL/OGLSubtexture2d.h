#pragma once

#include "OGLTexture2D.h"

#include "glm.hpp"

class GLSubTexture2D
{
public:
	GLSubTexture2D(OGLTexture2D& texture, const glm::vec2& position, const glm::vec2& cellsize, const glm::vec2& size = {1.0f, 1.0f});
	~GLSubTexture2D();

	inline glm::vec2& GetPosition() { return m_Position; }
	inline glm::vec2& GetCellSize() { return m_CellSize; }
	inline glm::vec2& GetSpriteSize() { return m_SpriteSize; }

	void SetPosition(const glm::vec2& position); 
	void SetSpriteSize(const glm::vec2& size); 
	void SetCellSize(const glm::vec2& size);
	inline OGLTexture2D* GetParentTexture() { return m_ParentTexture; }

	inline glm::vec2* GetTextureCoords() { return m_TextureCoords; }

private:
	OGLTexture2D* m_ParentTexture;
	glm::vec2 m_CellSize,m_SpriteSize;
	glm::vec2 m_Position;
	glm::vec2 m_TextureCoords[4];
};

