#pragma once

#include "World.h"

class Game {
private:
	static World* m_World;
	static Engine* m_Engine;

	enum class Scene : uint16_t {
		TITLE,
			SAVE_VIEWER,
				WORLD_CREATION,
					WORLD_LOADING,
			OPTIONS,
		GAME
	};

	static Scene m_CurrentScene;

	static void m_RenderTitleScene();
	static void m_RenderGame();

public:
	static void Init();
};