#pragma once

#include "glm.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "gtx/quaternion.hpp"

class PerspectiveCamera
{
	public:
		PerspectiveCamera() = default;
		PerspectiveCamera(float fov, float aspectratio, float nearclip, float farclip);

		void CalculateProjection();
		void UpdateView();
		glm::mat4 GetProjectionMatrix() { return m_ProjectionMatrix; }
		glm::mat4 GetViewMatrix() { return m_ViewMatrix; }
		glm::mat4 GetViewProjection() const { return m_ProjectionMatrix * m_ViewMatrix; }
		glm::quat GetOrientation() const;
		void SetOrientation(glm::quat orientation) { m_Orientation = orientation; }
		glm::vec3 GetForwardDirection() const;
		glm::vec3 CalculatePosition();
		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		//glm::vec3 GetForwardDirection() const;
		//const glm::vec3& GetPosition() const { return m_Position; }
		//glm::quat GetOrientation() const;

		void Translate(glm::vec3 position) { m_Position = position; }
		glm::vec3 GetPosition() { return m_Position; }
		void SetDistance(float distance) { m_Distance = distance; }
		float GetDistance() { return m_Distance; }
		void SetPitch(float pitch) { m_Pitch = pitch; }
		float GetPitch() { return m_Pitch; }
		void SetYaw(float yaw) { m_Yaw = yaw; }
		float GetYaw() { return m_Yaw; }
		void SetFOV(float fov) { m_FOV = fov; CalculateProjection();}
		float GetFOV() { return m_FOV; }
		void SetNearClip(float nearclip) { m_NearClip = nearclip; CalculateProjection(); }
		float GetNearClip() { return m_NearClip; }
		void SetFarClip(float farclip) { m_FarClip = farclip; CalculateProjection(); }
		float GetFarClip() { return m_FarClip; }
		void SetFocalPoint(glm::vec3 point) { m_FocalPoint = point; }
		glm::vec3 GetFocalPoint() { return m_FocalPoint; }
	private:
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };
		float m_Distance = 10.0f;
		float m_Pitch = 0.0f, m_Yaw = 0.0f;
		glm::quat m_Orientation;
		float m_FOV = 60.0f;
		float m_AspectRatio;
		float m_NearClip, m_FarClip;
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f), m_ProjectionMatrix = glm::mat4(1.0f);
};

