#include "Layer.h"

LayerStack::~LayerStack()
{
	for (auto layer : m_layers)
	{
		layer->OnDetach();
		delete layer;
	}
}

void LayerStack::PushLayer(Layer* layer)
{ 
	m_layers.emplace(m_layers.begin() + m_layer_count,  layer);
	m_layer_count++;
}

void LayerStack::PopLayer(Layer* layer)
{
	auto it = std::find(m_layers.begin(), m_layers.begin() + m_layer_count, layer);
	if (it != m_layers.begin() + m_layer_count)
	{
		layer->OnDetach();
		m_layers.erase(it);
		m_layer_count--;
	}
}