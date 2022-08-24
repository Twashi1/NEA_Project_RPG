#pragma once

#include "Core.h"
#include "Renderer.h"

namespace Vivium {
	class VIVIUM_API IScene {
	public:
		virtual ~IScene() = default;

		virtual void Render() = 0;
		virtual void Update() = 0;
	};
}
