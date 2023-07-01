#include "Application.h"

//#include "Vendor/OpenGL/opengl_impl.h"
#include "Log/Log.h"
#include "Graphics/ImGui/ImGuiCore.h"
#include "Timestep.h"
#include "imgui.h"

#include "glfw3.h"

#include "Core/Base.h"

Application* Application::s_Instance = nullptr;

Application::Application(const std::string name, const uint32_t width, const uint32_t height)
{
	m_name = name;
	m_width = width;
	m_height = height;

	s_Instance = this;
}


Application::~Application()
{
	ImGuiCore::Shutdown();
}

void Application::Init()
{
	if (!glfwInit())
	{
		LE_CORE_ERROR("Cannot initialize GLFW");
		return;
	}


	m_window = new WindowsWindow(m_name, m_width, m_height);
	
	m_Renderer = Renderer::Create(*m_window);

	Renderer2D::Init();
	ImGuiCore::Init(*m_window);

	m_window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

	m_Renderer->UseDepthTesting(true);

	LE_CORE_INFO("Application initialized!");
	LE_CORE_INFO("OpenGL version: " + m_Renderer->GetVersionString());
}


void Application::PushLayer(Layer* layer)
{
	m_layerstack.PushLayer(layer);
	layer->OnAttach();
}

void Application::OnEvent(Event& e)
{

	for (Layer* layer : m_layerstack)
	{
		if(!e.Handled) layer->OnEvent(e);
	}
}


void Application::Start()
{
	m_running = true;

	m_LastTime = (float)glfwGetTime();

	while (!m_window->WindowShouldClose())
	{
		float time = (float)glfwGetTime(); // Temporary, replace it with time abstraction
		Timestep timestep = time - m_LastTime;
		m_LastTime = (float)glfwGetTime();

		m_window->UpdateWindow();
		for (Layer* layer : m_layerstack)
		{

			
			{
				layer->OnUpdate(timestep);
			}

			{
				ImGuiCore::BeginFrame();
				layer->OnImGuiRender();
				ImGuiCore::EndFrame();
			}
		}
	}

	//OpenGL::Terminate();
}