#pragma once

#include <string>
#include <functional>

#include "Events/Events.h"

struct GLFWwindow;

#include "windows.h"

class WindowsWindow
{
public:
	WindowsWindow() = default;
	WindowsWindow(const std::string title, const uint32_t width, const uint32_t height);
	~WindowsWindow();

	void Destroy();
	void UpdateWindow();
	bool WindowShouldClose();
	void Resize(uint32_t width, uint32_t height);
	inline uint32_t GetWidth() { return m_WindowData.Width; }
	inline uint32_t GetHeight() { return m_WindowData.Height; }
	inline float GetTargetAspectRatio() { return m_WindowData.TargetAspectRatio; }

	inline GLFWwindow* GetNativeWindow() { return m_window; }


	inline void SetEventCallback(const std::function<void(Event&)>& callback) { m_WindowData.EventCallback = callback; }

private:
	GLFWwindow* m_window;

	struct WindowData
	{
		std::string Title;
		uint32_t Width, Height;

		uint32_t TargetWidth;
		uint32_t TargetHeight;
		float TargetAspectRatio;

		std::function<void(Event&)> EventCallback;
	};
	
	WindowData m_WindowData;
};