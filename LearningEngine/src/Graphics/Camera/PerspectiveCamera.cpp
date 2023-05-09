#include "PerspectiveCamera.h"

#include "gtc/matrix_transform.hpp"

PerspectiveCamera::PerspectiveCamera(float fov, float aspectratio, float nearclip, float farclip)
	:	m_FOV(fov),  m_AspectRatio(aspectratio), m_NearClip(nearclip), m_FarClip(farclip)
{
	CalculateProjection();
}

void PerspectiveCamera::CalculateProjection()
{
	m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
}

void PerspectiveCamera::UpdateView()
{
	//m_Position = CalculatePosition();

	glm::quat orientation = GetOrientation();
	m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
	m_ViewMatrix = glm::inverse(m_ViewMatrix);
}

glm::vec3 PerspectiveCamera::GetForwardDirection() const
{
	return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
}

glm::quat PerspectiveCamera::GetOrientation() const
{
	return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
}

glm::vec3 PerspectiveCamera::CalculatePosition()
{
	return m_FocalPoint - GetForwardDirection() * m_Distance;
}

glm::vec3 PerspectiveCamera::GetUpDirection() const
{
	return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 PerspectiveCamera::GetRightDirection() const
{
	return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
}
