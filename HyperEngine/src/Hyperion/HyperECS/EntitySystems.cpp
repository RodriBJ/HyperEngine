#include "EntitySystems.hpp"

namespace Hyperion
{
	EntitySystems::EntitySystems(Registry& registry, Ref<Renderer2D> renderer2D)
		: m_Registry(registry), m_Renderer2D(renderer2D)
	{
	}

	void EntitySystems::OnRender()
	{
		for (const auto& entitySystem : m_Systems)
			entitySystem.second->Render(m_Registry);
	}

	void EntitySystems::OnUpdate(Timestep timeStep)
	{
		for (const auto& entitySystem : m_Systems)
			entitySystem.second->Update(m_Registry, timeStep);
	}

	void EntitySystems::OnEvent(Event& event)
	{
		for (const auto& entitySystem : m_Systems)
			entitySystem.second->OnEvent(m_Registry, event);
	}
}
