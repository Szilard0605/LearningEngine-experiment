#pragma once

#include "Graphics/Renderer/API/Renderer.h"
#include "Graphics/Renderer/API/RenderContext.h"

#include "OGLVertexArray.h"
#include "Core/Base.h"

#include <cstdint>
#include "glm.hpp"

class OGLRenderer : public Renderer
{
public:
	OGLRenderer(WindowsWindow& window);
	~OGLRenderer();

	virtual RenderContext* GetContext() override;
	virtual std::string GetVersionString() override;
	virtual void Clear(const glm::vec4& color) override;
	virtual void Resize(const float width, const float height) override;
	virtual void Present() override;
	virtual void UseDepthTesting(bool enable) override;
	virtual void BindViewport() override;
	virtual void SetViewportSize(float width, float height) override;
	virtual glm::vec2 GetViewportSize() override;
	virtual void SetViewportPosition(float x, float y) override;
	virtual glm::vec2 GetViewportPosition() override;

private:
	glm::vec4 m_Viewport;
	WindowsWindow m_Window;
	RenderContext* m_Context;
};

