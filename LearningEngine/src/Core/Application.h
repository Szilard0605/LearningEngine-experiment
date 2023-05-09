#pragma once

#include "Base.h"
#include "Layers/Layer.h"

#include "PLatform/Windows/WindowsWindow.h"

#include <functional>

#include <Events/Event.h>

#include "Graphics/Renderer/API/Renderer.h"

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

		inline Renderer* GetRenderer() { return m_Renderer; }

	private:
		static Application* s_Instance;

		Renderer* m_Renderer;

		float m_LastTime = 0.0f;
		std::string m_name;
		uint32_t m_width, m_height;

		bool m_running = false;

		WindowsWindow* m_window;
		LayerStack m_layerstack;

};


