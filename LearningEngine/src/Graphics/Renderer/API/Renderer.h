#pragma once


#include "Platform/Windows/WindowsWindow.h"

#include "glm.hpp"
#include <string>

class RenderContext;

class Renderer
{
public:
	enum class API
	{
		None, D3D11, OpenGL
	};

	static Renderer* Create(WindowsWindow& window);

	virtual RenderContext* GetContext() = 0;
	virtual std::string GetVersionString() = 0;
	virtual void Clear(const glm::vec4& color) = 0;
	virtual void Resize(const int width, const int height) = 0;
	virtual void Present() = 0;
	virtual void UseDepthTesting(bool enable) = 0;
	virtual void BindViewport() = 0;
	virtual void SetViewportSize(int width, int height) = 0;
	virtual glm::vec2 GetViewportSize() = 0;
	virtual void SetViewportPosition(int x, int y) = 0;
	virtual glm::vec2 GetViewportPosition() = 0;

	static API GetAPI() { return s_API; }
private:
	static API s_API;
};