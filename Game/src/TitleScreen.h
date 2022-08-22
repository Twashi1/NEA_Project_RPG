#pragma once

#include "Core.h"
#include "World.h"
#include "Player.h"

namespace Game {
	class TitleScreen {
	private:
		Vivium::Sprite* m_TitleSprite;
		Vivium::Button* m_StartGameButton;

		static void m_StartGameCallback(Vivium::Button* button, void* params);
		void m_StartGame(Vivium::Button* button);

	public:
		TitleScreen();
		~TitleScreen();

		void Update();
		void Render();
	};
}
