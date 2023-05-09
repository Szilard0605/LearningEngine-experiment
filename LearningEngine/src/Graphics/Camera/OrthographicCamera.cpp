#include "OrthographicCamera.h"

#include "orthographiccamera.h"
#include "gtc/matrix_transform.hpp"

OrthographicCamera::OrthographicCamera(const float aspect_ratio, const glm::vec3 position, const float zoomlevel, const float rotation)
{
	m_position = position;
	m_rotation = rotation;
	m_zoomlevel = zoomlevel;
	m_aspect_ratio = aspect_ratio;

	CalculateViewMatrix();
	//SetProjection(-aspect_ratio * zoomlevel, aspect_ratio * zoomlevel, -zoomlevel, zoomlevel);
	SetProjection(-aspect_ratio * zoomlevel, aspect_ratio * zoomlevel, -zoomlevel, zoomlevel);
}

void OrthographicCamera::SetProjection(const float left, const float right, const float bottom, const float top)
{
	ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1000.0f);
	ViewProjectionMatrix = ViewMatrix * ProjectionMatrix;
}

void OrthographicCamera::CalculateViewMatrix()
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position) *
		glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(0, 0, 1));

	ViewMatrix = glm::inverse(transform);
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
}

void OrthographicCamera::SetPosition(const glm::vec3& position)
{
	m_position = position;
	CalculateViewMatrix();
}

void OrthographicCamera::SetRotation(const float rotation)
{
	m_rotation = rotation;
	CalculateViewMatrix();
}

void OrthographicCamera::SetAspectRatio(const float aspect_ratio)
{
	m_aspect_ratio = aspect_ratio;
	CalculateViewMatrix();
	SetProjection(-aspect_ratio * m_zoomlevel, aspect_ratio * m_zoomlevel, -m_zoomlevel, m_zoomlevel);
}

glm::vec2 OrthographicCamera::GetScreenCoordinates(const glm::vec2 WorldPosition, const glm::vec2 Viewport)
{
	glm::vec3 coords = glm::unProject(glm::vec3(WorldPosition, 0), GetViewMatrix(), GetProjectionMatrix(), glm::vec4(0.0f, 0.0f, Viewport));
	return glm::vec2(coords.x, -coords.y);
}

void OrthographicCamera::SetZoomLevel(const float zoomlevel)
{
	m_zoomlevel = zoomlevel;
	SetProjection(-m_aspect_ratio * m_zoomlevel, m_aspect_ratio * m_zoomlevel, -m_zoomlevel, m_zoomlevel);
	CalculateViewMatrix();
}