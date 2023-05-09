#pragma once

#include <glm.hpp>

#include "Core/Base.h"

class OrthographicCamera // ONLY INCLUDED IN API FOR TESTING
{
	private:
		glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 0.0f);
		float m_rotation = 0;
		float m_zoomlevel = 0;
		float m_aspect_ratio = 0;

	protected:
		glm::mat4 ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 ViewMatrix = glm::mat4(1.0f);
		glm::mat4 ViewProjectionMatrix = glm::mat4(1.0f);


	public:
		OrthographicCamera() = default;
		OrthographicCamera(const float aspect_ratio, const glm::vec3 position, const float zoomlevel, const float rotation);

		void SetProjection(const float left, const float right, const float bottom, const float top);
		void CalculateViewMatrix();
		void SetPosition(const glm::vec3& position);
		void SetRotation(const float rotation);
		void SetZoomLevel(const float zoomlevel);
		void SetAspectRatio(const float aspect_ratio);
		glm::vec2 GetScreenCoordinates(const glm::vec2 WorldPosition, const glm::vec2 Viewport);
		const float GetAspectRatio() { return m_aspect_ratio; }
		const float GetZoomLevel() { return m_zoomlevel; }
		glm::vec3 GetPosition() { return m_position; }
		const glm::mat4 GetProjectionMatrix() { return ProjectionMatrix; }
		const glm::mat4 GetViewMatrix() { return ViewMatrix; }
		const glm::mat4 GetViewProjection() { return ViewProjectionMatrix; }

};

