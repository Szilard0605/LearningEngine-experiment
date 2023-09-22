#pragma once

#include "Graphics/Renderer/API/RendererAPI.h"
#include "Graphics/Renderer/API/RenderContext.h"

#include "OGLVertexArray.h"
#include "Core/Base.h"

#include <cstdint>
#include "glm.hpp"

class OGLRendererAPI : public RendererAPI
{
public:
	OGLRendererAPI(WindowsWindow& window);
	~OGLRendererAPI();

	virtual RenderContext* GetContext() override;
	virtual std::string GetVersionString() override;
	virtual void Clear(const glm::vec4& color) override;
	virtual void Resize(const int width, const int height) override;
	virtual void Present() override;
	virtual void UseDepthTesting(bool enable) override;
	virtual void BindViewport() override;
	virtual void SetViewportSize(int width, int height) override;
	virtual glm::vec2 GetViewportSize() override;
	virtual void SetViewportPosition(int x, int y) override;
	virtual glm::vec2 GetViewportPosition() override;

private:
	glm::ivec4 m_Viewport;
	WindowsWindow m_Window;
	RenderContext* m_Context;
};

