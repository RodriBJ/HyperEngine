#pragma once

#include <queue>

#include "Core.hpp"
#include "HyperECS/Scene/Scene.hpp"
#include "HyperEvents/Event.hpp"
#include "HyperLayer/Layer.hpp"
#include "HyperLayer/LayerStack.hpp"
#include "HyperLayer/OverlayLayer.hpp"
#include "HyperRendering/Window.hpp"

namespace Hyperion 
{
	class Application
	{
	private:
		Ref<Window> m_Window;
		LayerStack* m_LayerStack;
		bool m_Running = true;

		std::queue<Ref<Event>> m_EventBus;

		Ref<Scene> m_Scene;

		static Application* m_Instance;

	public:
		Application();

		void Run();

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		void PopLayer(const std::string& layerName);
		void PopLayer();
		Layer* GetLayer(const std::string& layerName);

		void PushOverlayLayer(OverlayLayer* overlayLayer);
		void PopOverlayLayer(OverlayLayer* overlayLayer);
		void PopOverlayLayer(const std::string& layerName);
		void PopOverlayLayer();
		OverlayLayer* GetOverlayLayer(const std::string& layerName);

		Ref<Window> GetNativeWindow() const;
		static Application* Get();

	private:
		void Init();
		void Shutdown();
		void OnEvent(Event& event);
	};

	Application* CreateApplication();
}
