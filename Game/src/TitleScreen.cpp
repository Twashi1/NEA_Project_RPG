#include "TitleScreen.h"

namespace Game {
	void TitleScreen::m_StartGameCallback(Vivium::Button* button, void* params)
	{
		// TODO: difficult to extract params if multiple in void*, maybe some sort of
		//	void* array type in Utils?
		// Params contains a pointer to an instance of the title screen class
		TitleScreen* title_screen_instance = (TitleScreen*)params;
		// Use instance to call start game
		title_screen_instance->m_StartGame(button);
	}

	void TitleScreen::m_StartGame(Vivium::Button* button)
	{
		// TODO: Create a world instance
		// etc.
		LogInfo("Starting game from title screen");
	}

	TitleScreen::TitleScreen()
	{
		// TODO: use panel
		Vivium::Vector2<float> screen_center = Vivium::Application::GetScreenDim() / 2;
		m_TitleSprite = new Vivium::Sprite(screen_center, { 128, 32 }, "title.png");
		m_StartGameButton = new Vivium::Button(
			Vivium::Quad(screen_center, Vivium::Vector2<float>(400, 100)),
			&TitleScreen::m_StartGameCallback,
			std::string("Start Game"),
			(void*)this
		);
	}

	TitleScreen::~TitleScreen()
	{
		delete m_TitleSprite;
		delete m_StartGameButton;
	}

	void TitleScreen::Update()
	{
		// Vivium::Vector2<float> cursor_pos = Vivium::Input::GetCursorPos();
		m_StartGameButton->Update();
	}

	void TitleScreen::Render()
	{
		Vivium::Renderer::Submit(m_TitleSprite);
		Vivium::Renderer::Submit(m_StartGameButton);
	}
}
