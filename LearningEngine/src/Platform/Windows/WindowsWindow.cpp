
#include "WindowsWindow.h"
#include "Log/Log.h"
#include "Core/Input.h"
#include "Platform/OpenGL/OGLRenderer.h"

#include <glew.h>
#include <glfw3.h>

#include "Core/Application.h"

WindowsWindow::WindowsWindow(const std::string title, const uint32_t width, const uint32_t height)
{
	m_WindowData.Title = title;
	m_WindowData.Width = width;
	m_WindowData.Height = height;

	m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

	if (!m_window)
	{
		LE_CORE_ERROR("Cannot create GLFW window");

		glfwTerminate();
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	glfwMakeContextCurrent(m_window);
	glfwSetWindowUserPointer(m_window, &m_WindowData);

	glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) 
	{
		WindowData* nw = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

		int aspect_width = width;
		int aspect_height = (int)((float)aspect_width / nw->TargetAspectRatio);
		if (aspect_height > height)
		{
			aspect_height = height;
			aspect_width = (int)((float)aspect_height * nw->TargetAspectRatio);
		}

		int vpX = (int)((float)width / 2.0f) - (int)((float)aspect_width / 2.0f);
		int vpY = (int)((float)height / 2.0f) - (int)((float)aspect_height / 2.0f);

		//GLRenderer::SetViewport(vpX, vpY, aspect_width, aspect_height);
		Application::GetInstance()->GetRenderer()->SetViewportPosition(vpX, vpY);
		Application::GetInstance()->GetRenderer()->SetViewportSize(aspect_width, aspect_height);
	});

	glfwSetKeyCallback(m_window, [](GLFWwindow* window, const int key, const int scancode, const int action, const int mods)
	{
		WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

		KeyEvent keyevent(key, action);
		data->EventCallback(keyevent);

		Input::OnKeyChange(key, scancode, action, mods);
	});

	glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double pos_x, double pos_y) 
	{
		WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

		MouseMoveEvent mouseevent({pos_x, pos_y});
		data->EventCallback(mouseevent);

		Input::OnMouseMove(pos_x, pos_y);
	});

	glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, const int button, const int action, const int mods) 
	{
		WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

		MouseButtonEvent mousebtnevent(button, action);
		data->EventCallback(mousebtnevent);

		Input::OnMouseChange(button, action, mods);
	});
	glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset) 
	{
		WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

		MouseScrolledEvent scrollevent({xoffset, yoffset});
		data->EventCallback(scrollevent);

		Input::OnMouseScrolled(xoffset, yoffset);
	});

	GLFWvidmode vidmode = *glfwGetVideoMode(glfwGetPrimaryMonitor());
	m_WindowData.Width = vidmode.width;
	m_WindowData.Height = vidmode.height;
	m_WindowData.TargetAspectRatio = (float)m_WindowData.Width / (float)m_WindowData.Height;
	
	//Application::GetInstance()->GetRenderer()->SetViewportPosition(0, 0);
	//Application::GetInstance()->GetRenderer()->SetViewportSize(width, height);
	//glfwSetWindowSizeLimits(m_window, width, height, width, height);

}

WindowsWindow::~WindowsWindow()
{
	glfwDestroyWindow(m_window);
}

void WindowsWindow::Destroy()
{
	glfwDestroyWindow(m_window);
}

void WindowsWindow::UpdateWindow()
{
	//m_window->Update();

	glfwSwapBuffers(m_window);
	glfwPollEvents();
}


bool WindowsWindow::WindowShouldClose()
{
	return glfwWindowShouldClose(m_window);
}

void WindowsWindow::Resize(uint32_t width, uint32_t height)
{
	int aspect_width = width;
	int aspect_height = (int)((float)aspect_width / m_WindowData.TargetAspectRatio);
	if (aspect_height > height)
	{
		aspect_height = height;
		aspect_width = (int)((float)aspect_height * m_WindowData.TargetAspectRatio);
	}

	int vpX = (int)((float)width / 2.0f) - (int)((float)aspect_width / 2.0f);
	int vpY = (int)((float)height / 2.0f) - (int)((float)aspect_height / 2.0f);

	Application::GetInstance()->GetRenderer()->SetViewportPosition(vpX, vpY);
	Application::GetInstance()->GetRenderer()->SetViewportSize(aspect_width, aspect_height);
}
