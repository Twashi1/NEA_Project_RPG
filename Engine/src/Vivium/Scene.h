#pragma once

#include "Core.h"
#include "Renderer.h"

namespace Vivium {
	class VIVIUM_API IScene {
	protected:
		int m_SceneID;

	public:
		IScene(int scene_id);
		virtual ~IScene() = default;

		virtual void Render() = 0;
		virtual void Update() = 0;
	};
}
