#include "Application.hpp"

#include <GLFW/glfw3.h>

#include <chrono>

#include "HyperEvents/WindowEvents.hpp"
#include "HyperUtilities/Random.hpp"
#include "HyperUtilities/Timestep.hpp"

#include "Platform/Rendering/ImGui/EditorLayer.hpp"

namespace Hyperion
{
	Application* Application::m_Instance;

	Application::Application()
	{
		m_Instance = this;
		Init();
	}

	void Application::Init()
	{
		HP_CORE_INFO("");
		HP_CORE_INFO("_     _ __   __  _____  _______  ______ _______ __   _  ______ _____ __   _ _______");
		HP_CORE_INFO("|_____|   \\_/   |_____] |______ |_____/ |______ | \\  | |  ____   |   | \\  | |______");
		HP_CORE_INFO("|     |    |    |       |______ |    \\_ |______ |  \\_| |_____| __|__ |  \\_| |______");
		HP_CORE_INFO("");

		Random::Init();

		WindowPropsInfo windowPropsInfo{};
		windowPropsInfo.Title = "HyperEngine (x64 - $api $version)";
		windowPropsInfo.Width = 1920;
		windowPropsInfo.Height = 1080;
		windowPropsInfo.EventBus = &m_EventBus;

		m_Window = Window::Construct(windowPropsInfo);

		m_Scene = CreateRef<Scene>("Main Scene", m_Window->GetContext()->GetRenderer2D());

		m_LayerStack = new LayerStack();
		PushOverlayLayer(new EditorLayer(m_Scene));
	}

	void Application::Shutdown()
	{
		m_Running = false;

		for (Layer* layer : m_LayerStack->GetLayers())
		{
			layer->OnDetach();
			m_LayerStack->PopLayer(layer);
		}

		for (OverlayLayer* overlayLayer : m_LayerStack->GetOverlayLayers())
		{
			overlayLayer->OnDetach();
			m_LayerStack->PopOverlayLayer(overlayLayer);
		}

		delete m_LayerStack;
	}

	void Application::Run()
	{
		double lastFrame = 0.0;

		EditorLayer* editorLayer = static_cast<EditorLayer*>(m_LayerStack->GetOverlayLayer("Editor Layer"));
		editorLayer->InitCapture();
		while (m_Running)
		{
			double currentFrame = glfwGetTime();
			const Timestep timeStep = currentFrame - lastFrame;
			lastFrame = currentFrame;

			m_Window->OnPreUpdate();

			editorLayer->StartCapture();

			while (m_EventBus.size() > 0)
			{
				Ref<Event> e = m_EventBus.front();
				if (e->Handled) continue;
				OnEvent(*e);
				m_EventBus.pop();
			}

			for (Layer* layer : m_LayerStack->GetLayers())
			{
				layer->OnUpdate(timeStep);
				layer->OnRender();
			}

			m_Scene->OnUpdate(timeStep);
			m_Scene->OnRender();

			editorLayer->EndCapture();

			for (OverlayLayer* overlayLayer : m_LayerStack->GetOverlayLayers())
			{
				overlayLayer->OnUpdate(timeStep);
				overlayLayer->OnRender();
			}

			m_Window->OnUpdate(timeStep);
		}
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<WindowCloseEvent>([&](WindowCloseEvent& e)
			{
				Shutdown();
				return false;
			});

		for (Layer* layer : m_LayerStack->GetLayers())
			layer->OnEvent(event);

		for (OverlayLayer* overlayLayer : m_LayerStack->GetOverlayLayers())
			overlayLayer->OnEvent(event);

		m_Scene->OnEvent(event);
	}

	void Application::PushLayer(Layer* layer)
	{
		layer->m_RenderContext = m_Window->GetContext();
		m_LayerStack->PushLayer(layer);
	}

	void Application::PopLayer(Layer* layer)
	{
		m_LayerStack->PopLayer(layer);
	}

	void Application::PopLayer(const std::string& layerName)
	{
		m_LayerStack->PopLayer(layerName);
	}

	void Application::PopLayer()
	{
		m_LayerStack->PopLayer();
	}

	Layer* Application::GetLayer(const std::string& layerName)
	{
		return m_LayerStack->GetLayer(layerName);
	}

	void Application::PushOverlayLayer(OverlayLayer* overlayLayer)
	{
		overlayLayer->m_RenderContext = m_Window->GetContext();
		m_LayerStack->PushOverlayLayer(overlayLayer);
	}

	void Application::PopOverlayLayer(OverlayLayer* overlayLayer)
	{
		m_LayerStack->PopOverlayLayer(overlayLayer);
	}

	void Application::PopOverlayLayer(const std::string& layerName)
	{
		m_LayerStack->PopOverlayLayer(layerName);
	}

	void Application::PopOverlayLayer()
	{
		m_LayerStack->PopOverlayLayer();
	}

	OverlayLayer* Application::GetOverlayLayer(const std::string& layerName)
	{
		return m_LayerStack->GetOverlayLayer(layerName);
	}

	Ref<Window> Application::GetNativeWindow() const
	{
		return m_Window;
	}

	Application* Application::Get()
	{
		return m_Instance;
	}
}
