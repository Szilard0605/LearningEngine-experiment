#pragma once

//#include "Core/Base.h"

//#include "Vendor/OpenGL/opengl_renderer.h"
#include "Graphics/Renderer/Renderer2D.h"
#include "Core/Timestep.h"
#include "Events/Event.h"

class Layer
{
	public:
		Layer(std::string name) : m_name(name) { };
		virtual ~Layer() = default;

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate(Timestep timestep) {}; //Add timestep later
		virtual void OnEvent(Event& event) {};
		virtual void OnImGuiRender() {};

	private:
		std::string m_name;
};

class LayerStack
{
	public:
		LayerStack() {}
		~LayerStack();

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);

		std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_layers.end(); }
		std::vector<Layer*>::reverse_iterator rbegin() { return m_layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return m_layers.rend(); }

		std::vector<Layer*>::const_iterator begin() const { return m_layers.begin(); }
		std::vector<Layer*>::const_iterator end()	const { return m_layers.end(); }
		std::vector<Layer*>::const_reverse_iterator rbegin() const { return m_layers.rbegin(); }
		std::vector<Layer*>::const_reverse_iterator rend() const { return m_layers.rend(); }
		

	private:
		std::vector<Layer*> m_layers;
		uint32_t m_layer_count = 0;

		

};
