#include "MainMenu.h"

namespace Game {
	void MainMenu::s_BackButtonCallback(Vivium::Button* button, void* params)
	{
		MainMenu* main_menu_instance = (MainMenu*)params;
		main_menu_instance->m_BackButtonCallback(button);
	}

	void MainMenu::m_BackButtonCallback(Vivium::Button* button)
	{
		m_LoadScene(SceneID::START);
	}

	void MainMenu::s_CreateWorldCallback(Vivium::Button* button, void* params)
	{
		// Params contains a pointer to an instance of the title screen class
		MainMenu* title_screen_instance = (MainMenu*)params;
		// Use instance to call start game
		title_screen_instance->m_CreateWorldCallback(button);
	}

	void MainMenu::m_CreateWorldCallback(Vivium::Button* button)
	{
		m_LoadScene(SceneID::CREATE_WORLD);
	}

	void MainMenu::s_LoadWorldCallback(Vivium::Button* button, void* params)
	{
		MainMenu* main_menu = (MainMenu*)params;
		main_menu->m_LoadWorldCallback(button);
	}

	void MainMenu::m_LoadWorldCallback(Vivium::Button* button)
	{
		m_LoadScene(SceneID::LOAD_WORLD);
	}

	void MainMenu::s_OptionsCallback(Vivium::Button* button, void* params)
	{
		MainMenu* main_menu = (MainMenu*)params;
		main_menu->m_OptionsCallback(button);
	}

	void MainMenu::m_OptionsCallback(Vivium::Button* button)
	{
		m_LoadScene(SceneID::OPTIONS);
	}

	void MainMenu::m_LoadScene(const SceneID& id)
	{
		Vivium::IScene*& scene_ptr = m_Scenes.at(id);

		if (scene_ptr != nullptr) {
			LogWarn("Scene has already been loaded!");
		}
		else {
			switch (id) {
			case SceneID::START:		scene_ptr = new SceneType<MainMenu::SceneID::START>::type(this);		break;
			case SceneID::CREATE_WORLD: scene_ptr = new SceneType<MainMenu::SceneID::CREATE_WORLD>::type(this);	break;
			case SceneID::LOAD_WORLD:	scene_ptr = new SceneType<MainMenu::SceneID::LOAD_WORLD>::type(this);	break;
			case SceneID::GAME:			scene_ptr = new SceneType<MainMenu::SceneID::GAME>::type(0, "", true);	break;
			case SceneID::OPTIONS:		scene_ptr = new SceneType<MainMenu::SceneID::OPTIONS>::type(this);		break;
			default: LogWarn("Invalid scene ID"); break;
			}
		}

		m_CurrentSceneID = id;
	}

	void MainMenu::m_LoadScene(const SceneID& id, Vivium::IScene* scene)
	{
		Vivium::IScene*& scene_ptr = m_Scenes.at(id);

		if (scene_ptr != nullptr) {
			LogWarn("Scene has already been loaded!");
		}
		else {
			scene_ptr = scene;
		}

		m_CurrentSceneID = id;
	}

	void MainMenu::m_DeallocateUnusedScenes()
	{
		for (auto& [scene_id, scene_ptr] : m_Scenes) {
			if (scene_id != m_CurrentSceneID && scene_ptr != nullptr) {
				delete scene_ptr;

				scene_ptr = nullptr;
			}
		}
	}

	MainMenu::MainMenu()
		: m_CurrentSceneID(SceneID::START),
		m_Scenes({
			{ SceneID::START,			nullptr },
			{ SceneID::CREATE_WORLD,	nullptr },
			{ SceneID::LOAD_WORLD,		nullptr },
			{ SceneID::GAME,			nullptr },
			{ SceneID::OPTIONS,			nullptr }
			})
	{
		m_LoadScene(SceneID::START);
	}

	MainMenu::~MainMenu()
	{
		for (auto& [scene_id, scene_ptr] : m_Scenes) {
			if (scene_ptr != nullptr) {
				delete scene_ptr;
			}
		}
	}

	void MainMenu::Update()
	{
		Vivium::IScene*& scene_ptr = m_Scenes.at(m_CurrentSceneID);

		if (scene_ptr == nullptr) {
			LogWarn("Scene has not been loaded");
		}

		scene_ptr->Update();

		m_DeallocateUnusedScenes();
	}

	void MainMenu::Render()
	{
		Vivium::IScene*& scene_ptr = m_Scenes.at(m_CurrentSceneID);

		if (scene_ptr == nullptr) {
			LogWarn("Scene has not been loaded");
		}

		scene_ptr->Render();
	}

	StartScene::StartScene(MainMenu* menu)
		: m_Manager(menu)
	{
		m_TitleSprite = std::make_shared<Vivium::Sprite>(Vivium::Vector2<float>(0.0f, 100.0f), Vivium::Vector2<float>(256.0f, 256.0f), "title.png", true);
		
		m_CreateWorldButton = std::make_shared<Vivium::Button>(
			Vivium::Quad(0.0f, -50.0f, 400.0f, 100.0f),
			&MainMenu::s_CreateWorldCallback,
			std::string("New World"),
			(void*)menu
		);

		m_LoadWorldButton = std::make_shared<Vivium::Button>(
			Vivium::Quad(0.0f, -180.0f, 400.0f, 100.0f),
			&MainMenu::s_LoadWorldCallback,
			std::string("Load World"),
			(void*)menu
		);

		m_OptionsButton = std::make_shared<Vivium::Button>(
			Vivium::Quad(0.0f, -310.0f, 400.0f, 100.0f),
			&MainMenu::s_OptionsCallback,
			std::string("Options"),
			(void*)menu
		);

		Vivium::Application::window_panel->Anchor(Vivium::Panel::ANCHOR::CENTER, Vivium::Panel::ANCHOR::CENTER, m_TitleSprite);
		Vivium::Application::window_panel->Anchor(Vivium::Panel::ANCHOR::CENTER, Vivium::Panel::ANCHOR::CENTER, m_CreateWorldButton);
		Vivium::Application::window_panel->Anchor(Vivium::Panel::ANCHOR::CENTER, Vivium::Panel::ANCHOR::CENTER, m_LoadWorldButton);
		Vivium::Application::window_panel->Anchor(Vivium::Panel::ANCHOR::CENTER, Vivium::Panel::ANCHOR::CENTER, m_OptionsButton);
	}

	StartScene::~StartScene()
	{
	}

	void StartScene::Render()
	{
		Vivium::Renderer::Submit(m_TitleSprite.get());
		Vivium::Renderer::Submit(m_CreateWorldButton.get());
		Vivium::Renderer::Submit(m_LoadWorldButton.get());
		Vivium::Renderer::Submit(m_OptionsButton.get());
	}

	void StartScene::Update()
	{
		m_CreateWorldButton->Update();
		m_LoadWorldButton->Update();
		m_OptionsButton->Update();
	}

	void CreateWorldScene::s_ConfirmCallback(Vivium::Button* button, void* user_params)
	{
		CreateWorldScene* instance = (CreateWorldScene*)(*(uintptr_t*)user_params);
		void* other_params = (char*)user_params + sizeof(CreateWorldScene*);

		instance->m_ConfirmCallback(button, other_params);
	}

	void CreateWorldScene::m_ConfirmCallback(Vivium::Button* button, void* user_params)
	{
		m_Name = m_NameInputBox->GetText()->GetText();

		// Get names of worlds that already exist
		std::string path = Vivium::Application::resources_path + "saves/";

		for (auto& it : std::filesystem::directory_iterator(path)) {
			if (it.is_directory()) {
				std::string world_name = it.path().stem().string(); // stem gives us the last part only

				if (m_Name == world_name) {
					m_WorldAlreadyExistsLifespan = s_WorldAlreadyExistsMaxLifespan; // Display for 3 seconds
					m_WorldAlreadyExistsTimer.Start();
					return;
				}
			}
		}

		std::string seed_text = m_SeedInputBox->GetText()->GetText();

		if (seed_text.empty()) {
			m_Seed = Vivium::Random::GetInt(0, INT_MAX);
		}
		else {
			m_Seed = std::hash<std::string>()(seed_text);
		}

		// Get MainMenu instance from params
		MainMenu* main_menu = (MainMenu*)(*(uintptr_t*)user_params);

		// Create instance of game scene
		// NOTE: main menu will take care of freeing this
		GameScene* game_scene = new GameScene(m_Seed, m_Name, true);

		// Start game
		main_menu->m_LoadScene(MainMenu::SceneID::GAME, game_scene);
	}

	CreateWorldScene::CreateWorldScene(MainMenu* menu)
	{
		m_BackButton = std::make_shared<Vivium::Button>(
			Vivium::Quad(-200.0f, 100.0f, 200.0f, 100.0f),
			&MainMenu::s_BackButtonCallback,
			"Go Back",
			menu
		);

		m_WorldAlreadyExistsText = std::make_shared<Vivium::Text>(
			"World already exists",
			Vivium::Vector2<float>(0.0f, -75.0f),
			Vivium::Text::Alignment::CENTER,
			0.25f
		);

		m_WorldAlreadyExistsShader = std::make_shared<Vivium::Shader>("text_vertex", "text_frag");
		m_WorldAlreadyExistsText->shader = m_WorldAlreadyExistsShader;

		m_WorldAlreadyExistsTimer.Start();

		std::size_t __size_unused = 0;

		uintptr_t intaddr0 = reinterpret_cast<uintptr_t>(this);
		uintptr_t intaddr1 = reinterpret_cast<uintptr_t>(menu);

		// TODO: needs some fixing, we're writing the address of the address or something like that
		params = Vivium::Utils::VoidArray::Make(__size_unused, intaddr0, intaddr1);

		m_NameInputBox = std::make_shared<Vivium::TextInput>(
			Vivium::Quad(0.0f, 300.0f, 300.0f, 100.0f),
			nullptr,
			(void*)this
		);

		m_SeedInputBox = std::make_shared<Vivium::TextInput>(
			Vivium::Quad(0.0f, 150.0f, 300.0f, 100.0f),
			nullptr,
			(void*)this
		);

		m_ConfirmButton = std::make_shared<Vivium::Button>(
			Vivium::Quad(0.0f, 0.0f, 300.0f, 100.0f),
			&CreateWorldScene::s_ConfirmCallback,
			std::string("Create World"),
			params
		);

		Vivium::Application::window_panel->Anchor(Vivium::Panel::ANCHOR::RIGHT, Vivium::Panel::ANCHOR::BOTTOM, m_BackButton);
		Vivium::Application::window_panel->Anchor(Vivium::Panel::ANCHOR::CENTER, Vivium::Panel::ANCHOR::CENTER, m_NameInputBox);
		Vivium::Application::window_panel->Anchor(Vivium::Panel::ANCHOR::CENTER, Vivium::Panel::ANCHOR::CENTER, m_SeedInputBox);
		Vivium::Application::window_panel->Anchor(Vivium::Panel::ANCHOR::CENTER, Vivium::Panel::ANCHOR::CENTER, m_ConfirmButton);
		Vivium::Application::window_panel->Anchor(Vivium::Panel::ANCHOR::CENTER, Vivium::Panel::ANCHOR::CENTER, m_WorldAlreadyExistsText);
	}

	CreateWorldScene::~CreateWorldScene()
	{
		free(params);
	}

	void CreateWorldScene::Render()
	{
		Vivium::Renderer::Submit(m_BackButton.get());
		Vivium::Renderer::Submit(m_NameInputBox.get());
		Vivium::Renderer::Submit(m_SeedInputBox.get());
		Vivium::Renderer::Submit(m_ConfirmButton.get());

		if (m_WorldAlreadyExistsLifespan > 0.0f) {
			float ratio = m_WorldAlreadyExistsLifespan / s_WorldAlreadyExistsMaxLifespan;

			m_WorldAlreadyExistsShader->Bind();
			m_WorldAlreadyExistsShader->SetUniform3f("u_TextColor", Vivium::RGBColor::RED);

			if (ratio < s_WorldAlreadyExistsFadeout) {
				ratio /= s_WorldAlreadyExistsFadeout;

				m_WorldAlreadyExistsShader->SetUniform1f("u_Alpha", ratio);
			}

			Vivium::Renderer::Submit(m_WorldAlreadyExistsText.get());

			m_WorldAlreadyExistsLifespan -= m_WorldAlreadyExistsTimer.GetElapsed();
		}
	}

	void CreateWorldScene::Update()
	{
		m_BackButton->Update();
		m_NameInputBox->Update();
		m_SeedInputBox->Update();
		m_ConfirmButton->Update();
	}

	GameScene::GameScene(uint32_t world_seed, const std::string& world_name, bool new_world)
	{
		if (new_world) {
			LogInfo("Creating world with seed {}, and name {}", world_seed, world_name);
			m_Player = new Player();
			m_World = new World(world_seed, world_name, m_Player);
		}
		else {
			LogInfo("Loading world with name {}", world_name);
			// TODO: shouldn't take seed
			m_Player = new Player(world_name);
			m_World = new World(world_seed, world_name, m_Player);
		}

		Vivium::Application::SetCursor(Vivium::Application::CURSOR_TYPE::POINTER);
	}

	GameScene::~GameScene()
	{
		delete m_World;
		delete m_Player;
	}

	void GameScene::Render()
	{
		Vivium::Vector2<float> update_pos = (m_Player->quad->GetCenter() / World::PIXEL_SCALE).floor();
		m_World->Render(update_pos);
		m_Player->Render();

		Vivium::Application::UpdateStats(*m_Player->body);
	}

	void GameScene::Update()
	{
		Vivium::Renderer::camera->SetCamera(
			m_Player->quad->GetCenter(),
			{ Vivium::Application::width / 2.0f, Vivium::Application::height / 2.0f },
			1.0f,
			0.0f
		);

		m_Player->Update(*m_World);
		m_World->Update();
	}

	LoadWorldScene::VisualWorldSelectable::VisualWorldSelectable(const Vivium::Vector2<float> pos, const std::string& world_name, LoadWorldScene* world_scene)
		: world_name(world_name)
	{
		params = new __CallbackData(world_name, world_scene);

		select_button = std::make_shared<Vivium::Button>(
			Vivium::Quad(pos, { 200.0f, 100.0f }),
			&LoadWorldScene::s_SelectedWorldCallback,
			world_name,
			(void*)params
		);

		world_scene->m_WorldsPanel->Anchor(Vivium::Panel::ANCHOR::CENTER, Vivium::Panel::ANCHOR::TOP, select_button);
	}
	
	LoadWorldScene::VisualWorldSelectable::~VisualWorldSelectable() {}

	void LoadWorldScene::s_SelectedWorldCallback(Vivium::Button* button, void* user_params)
	{
		__CallbackData* callback_data = (__CallbackData*)user_params;
			
		callback_data->scene->m_SelectedWorldCallback(button, user_params);
	}

	void LoadWorldScene::m_SelectedWorldCallback(Vivium::Button* button, void* user_params)
	{
		__CallbackData* callback_data = (__CallbackData*)user_params;

		std::string world_name = callback_data->world_name;

		// TODO: world seed needs to be stored
		GameScene* game = new GameScene(0U, world_name, false);

		m_Manager->m_LoadScene(MainMenu::SceneID::GAME, game);
	}

	LoadWorldScene::LoadWorldScene(MainMenu* menu)
		: m_Manager(menu)
	{
		m_BackButton = std::make_shared<Vivium::Button>(
			Vivium::Quad(-200.0f, 100.0f, 200.0f, 100.0f),
			&MainMenu::s_BackButtonCallback,
			"Go Back",
			menu
		);

		m_WorldsPanel = std::make_shared<Vivium::Panel>(
			Vivium::Quad(Vivium::Application::width / 2, Vivium::Application::height / 2, Vivium::Application::width, Vivium::Application::height)
		);

		Vivium::Application::window_panel->Anchor(Vivium::Panel::ANCHOR::RIGHT, Vivium::Panel::ANCHOR::BOTTOM, m_BackButton);

		std::string path = Vivium::Application::resources_path + "saves/";
		Vivium::Vector2<float> current_offset = {0.0f, -100.0f};

		float lowest_offset = INT_MIN;

		for (auto& it : std::filesystem::directory_iterator(path)) {
			if (it.is_directory()) {
				std::string world_name = it.path().stem().string(); // stem gives us the last part only

				m_Worlds.emplace_back(current_offset, world_name, this);

				LogTrace("Found world name: {}", world_name);
				
				// TODO: magic number here
				current_offset.y -= 130.0f;

				lowest_offset = current_offset.y;
			}
		}

		// TODO: scrolling needs work
		m_WorldsPanel->SetScrollLimits(0.0f, -Vivium::Application::height - current_offset.y);
	}

	LoadWorldScene::~LoadWorldScene()
	{
		for (VisualWorldSelectable& selectable : m_Worlds) {
			delete selectable.params;
		}
	}

	void LoadWorldScene::Render()
	{
		Vivium::Renderer::Submit(m_BackButton.get());

		for (VisualWorldSelectable& selectable : m_Worlds) {
			Vivium::Renderer::Submit(selectable.select_button.get());
		}
	}

	void LoadWorldScene::Update()
	{
		m_BackButton->Update();
		m_WorldsPanel->SetDim({ (float)Vivium::Application::width, (float)Vivium::Application::height });
		m_WorldsPanel->Update();

		for (VisualWorldSelectable& selectable : m_Worlds) {
			selectable.select_button->Update();
		}
	}

	LoadWorldScene::__CallbackData::__CallbackData(const std::string& world_name, LoadWorldScene* scene)
		: world_name(world_name), scene(scene)
	{
	}

	void OptionsScene::s_FPSCallback(Vivium::Slider* slider, void* user_params)
	{
		Vivium::Application::SetFPS(slider->GetValue(FPS_MIN, FPS_MAX));
	}

	void OptionsScene::s_VolumeCallback(Vivium::Slider* slider, void* user_params)
	{
		Vivium::Application::SetVolume(slider->GetValue());
	}

	OptionsScene::OptionsScene(MainMenu* manager)
		: m_Manager(manager)
	{
		m_BackButton = std::make_shared<Vivium::Button>(
			Vivium::Quad(-200.0f, 100.0f, 200.0f, 100.0f),
			&MainMenu::s_BackButtonCallback,
			"Go Back",
			manager
		);

		m_FPSSlider = std::make_shared<Vivium::Slider>(
			std::make_shared<Vivium::Quad>(0.0f, 200.0f, 400.0f, 30.0f),
			std::make_shared<Vivium::Quad>(0.0f, 200.0f, 20.0f, 40.0f),
			&OptionsScene::s_FPSCallback
		);

		m_FPSText = std::make_shared<Vivium::Text>(
			"FPS: 0",
			Vivium::Vector2<float>(-200.0f, 230.0f),
			Vivium::Text::Alignment::LEFT,
			0.25f
		);

		m_VolumeSlider = std::make_shared<Vivium::Slider>(
			std::make_shared<Vivium::Quad>(0.0f, 100.0f, 400.0f, 30.0f),
			std::make_shared<Vivium::Quad>(0.0f, 100.0f, 20.0f, 40.0f),
			&OptionsScene::s_VolumeCallback
		);

		m_VolumeText = std::make_shared<Vivium::Text>(
			"Volume: 100%",
			Vivium::Vector2<float>(-200.0f, 130.0f),
			Vivium::Text::Alignment::LEFT,
			0.25f
		);

		// TODO: scroll limits when more options
		m_OptionsPanel = std::make_shared<Vivium::Panel>(
			std::make_shared<Vivium::Quad>(0.0f, 0.0f, 500.0f, 1000.0f)
		);

		Vivium::Application::window_panel->Anchor(Vivium::Panel::ANCHOR::RIGHT, Vivium::Panel::ANCHOR::BOTTOM, m_BackButton);

		m_OptionsPanel->Anchor(Vivium::Panel::ANCHOR::CENTER, Vivium::Panel::ANCHOR::CENTER, m_FPSSlider);
		m_OptionsPanel->Anchor(Vivium::Panel::ANCHOR::CENTER, Vivium::Panel::ANCHOR::CENTER, m_FPSText);
		m_OptionsPanel->Anchor(Vivium::Panel::ANCHOR::CENTER, Vivium::Panel::ANCHOR::CENTER, m_VolumeSlider);
		m_OptionsPanel->Anchor(Vivium::Panel::ANCHOR::CENTER, Vivium::Panel::ANCHOR::CENTER, m_VolumeText);
		Vivium::Application::window_panel->Anchor(Vivium::Panel::ANCHOR::CENTER, Vivium::Panel::ANCHOR::CENTER, m_OptionsPanel);
	}

	void OptionsScene::Render()
	{
		Vivium::Renderer::Submit(m_BackButton.get());
		Vivium::Renderer::Submit(m_FPSSlider.get());
		Vivium::Renderer::Submit(m_FPSText.get());
		Vivium::Renderer::Submit(m_VolumeSlider.get());
		Vivium::Renderer::Submit(m_VolumeText.get());
	}

	void OptionsScene::Update()
	{
		m_BackButton->Update();
		m_FPSSlider->Update();
		m_VolumeSlider->Update();

		int new_fps = m_FPSSlider->GetValue(FPS_MIN, FPS_MAX);
		m_FPSText->SetText(std::format("FPS: {}", new_fps));

		// As percentage
		int new_volume = m_VolumeSlider->GetValue(100.0f);
		m_VolumeText->SetText(std::format("Volume: {}%", new_volume));
	}
}
