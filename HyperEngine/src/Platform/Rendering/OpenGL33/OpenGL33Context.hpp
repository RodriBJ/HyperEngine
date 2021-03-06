#pragma once

#include "HyperRendering/RenderContext.hpp"

namespace Hyperion
{
	class OpenGL33Context : public RenderContext
	{
	public:
		OpenGL33Context(GraphicsAPI graphicsAPI);

		virtual void PreInit() override;
		virtual void Init() override;
		virtual void Shutdown() override;

		virtual void OnPreUpdate() override;
		virtual void OnUpdate(Timestep timeStep) override;
		virtual void OnRender() override;
	};
}
