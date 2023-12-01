#pragma once

#include "Base.h"
#include "Layers/Layer.h"
#include "PLatform/Windows/WindowsWindow.h"
#include <functional>
#include <Events/Event.h>
#include "Graphics/Renderer/API/RendererAPI.h"

#include "Physics/PhysicsCore.h"

class Application
{
public:
	Application(const std::string name, const uint32_t width, const uint32_t height);

	virtual ~Application();
	virtual void Init();
	void Start();
	void PushLayer(Layer* layer);
	void OnEvent(Event& e);
	inline WindowsWindow* GetWindow() { return m_window; }
	inline static Application* GetInstance() { return s_Instance; }

	inline RendererAPI* GetRenderer() { return m_RendererAPI; }

	PhysicsCore* GetPhysicsCore() { return m_PhysicsCore; }

	private:
		static Application* s_Instance;

		RendererAPI* m_RendererAPI;

		float m_LastTime = 0.0f;
		std::string m_name;
		uint32_t m_width, m_height;

		bool m_running = false;

		WindowsWindow* m_window;
		LayerStack m_layerstack;

		PhysicsCore* m_PhysicsCore;

};


