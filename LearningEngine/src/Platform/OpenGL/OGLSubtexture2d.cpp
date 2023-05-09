#include "OGLSubtexture2d.h"

/*GLSubTexture2D::GLSubTexture2D(OGLTexture2D& texture, const glm::vec2& position, const glm::vec2& cellsize, const glm::vec2& size)
	: m_ParentTexture(&texture), m_Position(position), m_CellSize(cellsize), m_SpriteSize(size)
{

	m_TextureCoords[0] = { (position.x * cellsize.x) / texture.width, (position.y * cellsize.y) / texture.height };
	m_TextureCoords[1] = { ((position.x + size.x) * cellsize.x) / texture.width, (position.y * cellsize.y) / texture.height };
	m_TextureCoords[2] = { ((position.x + size.x) * cellsize.x) / texture.width, ((position.y + size.y) * cellsize.y) / texture.height };
	m_TextureCoords[3] = { (position.x * cellsize.x) / texture.width, ((position.y + size.y) * cellsize.y) / texture.height };
	
}

GLSubTexture2D::~GLSubTexture2D()
{

}

void GLSubTexture2D::SetPosition(const glm::vec2& position)
{
	m_Position = position;

	m_TextureCoords[0] = { (position.x * m_CellSize.x) / m_ParentTexture->width, (position.y * m_CellSize.y) / m_ParentTexture->height };
	m_TextureCoords[1] = { ((position.x + m_SpriteSize.x) * m_CellSize.x) / m_ParentTexture->width, (position.y * m_CellSize.y) / m_ParentTexture->height };
	m_TextureCoords[2] = { ((position.x + m_SpriteSize.x) * m_CellSize.x) / m_ParentTexture->width, ((position.y + m_SpriteSize.y) * m_CellSize.y) / m_ParentTexture->height };
	m_TextureCoords[3] = { (position.x * m_CellSize.x) / m_ParentTexture->width, ((position.y + m_SpriteSize.y) * m_CellSize.y) / m_ParentTexture->height };
}

void GLSubTexture2D::SetSpriteSize(const glm::vec2& size)
{
	m_SpriteSize = size;
}

void GLSubTexture2D::SetCellSize(const glm::vec2& size)
{
	m_CellSize = size;
}*/

