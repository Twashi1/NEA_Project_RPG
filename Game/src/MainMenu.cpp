#include "MainMenu.h"

namespace Game {
	void MainMenu::m_StartGameCallback(Vivium::Button* button, void* params)
	{
		// TODO: difficult to extract params if multiple in void*, maybe some sort of
		//	void* array type in Utils?
		// Params contains a pointer to an instance of the title screen class
		MainMenu* title_screen_instance = (MainMenu*)params;
		// Use instance to call start game
		title_screen_instance->m_StartGame(button);
	}

	void MainMenu::m_StartGame(Vivium::Button* button)
	{
		m_LoadScene(SceneID::GAME);
	}

	void MainMenu::m_LoadScene(const SceneID& id)
	{
		Vivium::IScene*& scene_ptr = m_Scenes.at(id);
		
		if (scene_ptr != nullptr) {
			LogWarn("Scene has already been loaded!");
		}
		else {
			switch (id) {
			case SceneID::START:		scene_ptr = new SceneType<MainMenu::SceneID::START>::type(			(int)id, this); break;
			case SceneID::CREATE_WORLD: scene_ptr = new SceneType<MainMenu::SceneID::CREATE_WORLD>::type(	(int)id); break;
			case SceneID::GAME:			scene_ptr = new SceneType<MainMenu::SceneID::GAME>::type(			(int)id); break;
			default: LogWarn("Invalid scene ID"); break;
			}
		}

		m_CurrentSceneID = id;
	}

	void MainMenu::m_DeallocateUnusedScenes()
	{
		for (auto& [scene_id, scene_ptr] : m_Scenes) {
			if (scene_id != m_CurrentSceneID && scene_ptr != nullptr) {
				switch (scene_id) {
				case SceneID::START:		{ ((SceneType<SceneID::START>::type*)			scene_ptr)->~__StartScene();		break; }
				case SceneID::CREATE_WORLD: { ((SceneType<SceneID::CREATE_WORLD>::type*)	scene_ptr)->~__CreateWorldScene();	break; }
				case SceneID::GAME:			{ ((SceneType<SceneID::GAME>::type*)			scene_ptr)->~__GameScene();			break; }
				default: LogWarn("Invalid scene ID"); break;
				
				// NOTE: idk if this actually frees the memory, since we called the deconstructor first
				delete scene_ptr;
				scene_ptr = nullptr;
				}
			}
		}
	}

	MainMenu::MainMenu()
		: m_CurrentSceneID(SceneID::START),
		m_Scenes({
			{ SceneID::START,			nullptr },
			{ SceneID::CREATE_WORLD,	nullptr },
			{ SceneID::GAME,			nullptr }
			})
	{
		m_LoadScene(SceneID::START);
	}

	MainMenu::~MainMenu()
	{
		for (auto& [scene_id, scene_ptr] : m_Scenes) {
			if (scene_ptr != nullptr) {
				switch (scene_id) {
				case SceneID::START:		delete ((SceneType<SceneID::START>::type*)			scene_ptr); break;
				case SceneID::CREATE_WORLD: delete ((SceneType<SceneID::CREATE_WORLD>::type*)	scene_ptr); break;
				case SceneID::GAME:			delete ((SceneType<SceneID::GAME>::type*)			scene_ptr); break;
				default: LogWarn("Invalid scene ID"); break;
				}
			}
		}
	}

	void MainMenu::Update()
	{
		Vivium::IScene*& scene_ptr = m_Scenes.at(m_CurrentSceneID);

		if (scene_ptr == nullptr) {
			LogWarn("Scene has not been loaded");
		}

		switch (m_CurrentSceneID) {
		case SceneID::START:		((SceneType<SceneID::START>::type*)			scene_ptr)->Update(); break;
		case SceneID::CREATE_WORLD: ((SceneType<SceneID::CREATE_WORLD>::type*)	scene_ptr)->Update(); break;
		case SceneID::GAME:			((SceneType<SceneID::GAME>::type*)			scene_ptr)->Update(); break;
		default: LogWarn("Invalid scene ID"); break;
		}

		m_DeallocateUnusedScenes();
	}

	void MainMenu::Render()
	{
		Vivium::IScene*& scene_ptr = m_Scenes.at(m_CurrentSceneID);

		if (scene_ptr == nullptr) {
			LogWarn("Scene has not been loaded");
		}

		switch (m_CurrentSceneID) {
		case SceneID::START:		((SceneType<SceneID::START>::type*)			scene_ptr)->Render(); break;
		case SceneID::CREATE_WORLD: ((SceneType<SceneID::CREATE_WORLD>::type*)	scene_ptr)->Render(); break;
		case SceneID::GAME:			((SceneType<SceneID::GAME>::type*)			scene_ptr)->Render(); break;
		default: LogWarn("Invalid scene ID"); break;
		}
	}

	__StartScene::__StartScene(int scene_id, MainMenu* menu)
		: Vivium::IScene(scene_id)
	{
		m_TitleSprite = MakeRef(Vivium::Sprite, Vivium::Vector2<float>(0.0f, 100.0f), Vivium::Vector2<float>(256.0f, 256.0f), "title.png", true);
		m_StartGameButton = MakeRef(Vivium::Button,
			Vivium::Quad(Vivium::Vector2<float>(0.0f, -50.0f), Vivium::Vector2<float>(400, 100)),
			&MainMenu::m_StartGameCallback,
			std::string("Start Game"),
			(void*)menu
		);

		Vivium::Application::window_panel->Anchor(Vivium::Panel::ANCHOR::CENTER, Vivium::Panel::ANCHOR::CENTER, m_TitleSprite);
		Vivium::Application::window_panel->Anchor(Vivium::Panel::ANCHOR::CENTER, Vivium::Panel::ANCHOR::CENTER, m_StartGameButton);
	}

	__StartScene::~__StartScene()
	{
	}

	void __StartScene::Render()
	{
		Vivium::Renderer::Submit(m_TitleSprite.get());
		Vivium::Renderer::Submit(m_StartGameButton.get());
	}
	
	void __StartScene::Update()
	{
		m_StartGameButton->Update();
	}

	__CreateWorldScene::__CreateWorldScene(int scene_id)
		: Vivium::IScene(m_SceneID)
	{
		// TODO
	}

	__CreateWorldScene::~__CreateWorldScene()
	{
	}

	void __CreateWorldScene::Render()
	{
	// TODO
	}

	void __CreateWorldScene::Update()
	{
	// TODO
	}

	__GameScene::__GameScene(int scene_id)
		: Vivium::IScene(m_SceneID)
	{
		m_World = new World(0, "getnamehere");
		m_Player = new Player();
	}

	__GameScene::~__GameScene()
	{
		delete m_World;
		delete m_Player;
	}

	void __GameScene::Render()
	{
		Vivium::Vector2<float> update_pos = (m_Player->quad->GetCenter() / World::PIXEL_SCALE).floor();
		m_World->Render(update_pos);
		m_Player->Render();

		Vivium::Application::UpdateStats(*m_Player->body);
	}

	void __GameScene::Update()
	{
		Vivium::Renderer::camera->SetCamera(
			m_Player->quad->GetCenter(),
			{ Vivium::Application::width / 2.0f, Vivium::Application::height / 2.0f },
			1.0f,
			0.0f
		);

		m_Player->Update(*m_World);
		m_World->Update(m_Player);
	}
}
