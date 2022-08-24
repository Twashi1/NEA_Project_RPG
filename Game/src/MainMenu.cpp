#include "MainMenu.h"

namespace Game {
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
			case SceneID::GAME:			scene_ptr = new SceneType<MainMenu::SceneID::GAME>::type(0, "");		break;
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
				switch (scene_id) {
				case SceneID::START:		{ delete ((SceneType<SceneID::START>::type*)		scene_ptr);	break; }
				case SceneID::CREATE_WORLD: { delete ((SceneType<SceneID::CREATE_WORLD>::type*)	scene_ptr);	break; }
				case SceneID::LOAD_WORLD:	{ delete ((SceneType<SceneID::LOAD_WORLD>::type*)	scene_ptr); break; }
				case SceneID::GAME:			{ delete ((SceneType<SceneID::GAME>::type*)			scene_ptr);	break; }
				default: LogWarn("Invalid scene ID"); break;
				}

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
				case SceneID::LOAD_WORLD:	delete ((SceneType<SceneID::LOAD_WORLD>::type*)		scene_ptr); break;
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
		case SceneID::LOAD_WORLD:	((SceneType<SceneID::LOAD_WORLD>::type*)	scene_ptr)->Update(); break;
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
		case SceneID::LOAD_WORLD:	((SceneType<SceneID::LOAD_WORLD>::type*)	scene_ptr)->Render(); break;
		case SceneID::GAME:			((SceneType<SceneID::GAME>::type*)			scene_ptr)->Render(); break;
		default: LogWarn("Invalid scene ID"); break;
		}
	}

	__StartScene::__StartScene(MainMenu* menu)
		: m_Manager(menu)
	{
		m_TitleSprite = MakeRef(Vivium::Sprite, Vivium::Vector2<float>(0.0f, 100.0f), Vivium::Vector2<float>(256.0f, 256.0f), "title.png", true);
		
		m_CreateWorldButton = MakeRef(Vivium::Button,
			Vivium::Quad(0.0f, -50.0f, 400.0f, 100.0f),
			&MainMenu::s_CreateWorldCallback,
			std::string("New World"),
			(void*)menu
		);

		m_LoadWorldButton = MakeRef(Vivium::Button,
			Vivium::Quad(0.0f, -180.0f, 400.0f, 100.0f),
			&MainMenu::s_LoadWorldCallback,
			std::string("Load World"),
			(void*)menu
			);

		Vivium::Application::window_panel->Anchor(Vivium::Panel::ANCHOR::CENTER, Vivium::Panel::ANCHOR::CENTER, m_TitleSprite);
		Vivium::Application::window_panel->Anchor(Vivium::Panel::ANCHOR::CENTER, Vivium::Panel::ANCHOR::CENTER, m_CreateWorldButton);
		Vivium::Application::window_panel->Anchor(Vivium::Panel::ANCHOR::CENTER, Vivium::Panel::ANCHOR::CENTER, m_LoadWorldButton);
	}

	__StartScene::~__StartScene()
	{
	}

	void __StartScene::Render()
	{
		Vivium::Renderer::Submit(m_TitleSprite.get());
		Vivium::Renderer::Submit(m_CreateWorldButton.get());
		Vivium::Renderer::Submit(m_LoadWorldButton.get());
	}

	void __StartScene::Update()
	{
		m_CreateWorldButton->Update();
		m_LoadWorldButton->Update();
	}

	void __CreateWorldScene::s_NameInputCallback(Vivium::TextInput* input_box, void* user_params)
	{
		__CreateWorldScene* instance = (__CreateWorldScene*)user_params;

		instance->m_NameInputCallback(input_box);
	}

	void __CreateWorldScene::s_SeedInputCallback(Vivium::TextInput* input_box, void* user_params)
	{
		__CreateWorldScene* instance = (__CreateWorldScene*)user_params;

		instance->m_SeedInputCallback(input_box);
	}

	void __CreateWorldScene::s_ConfirmCallback(Vivium::Button* button, void* user_params)
	{
		__CreateWorldScene* instance = (__CreateWorldScene*)(*(uintptr_t*)user_params);
		void* other_params = (char*)user_params + sizeof(__CreateWorldScene*);

		instance->m_ConfirmCallback(button, other_params);
	}

	void __CreateWorldScene::m_NameInputCallback(Vivium::TextInput* input_box)
	{
		m_Name = input_box->GetText()->text;
	}

	void __CreateWorldScene::m_SeedInputCallback(Vivium::TextInput* input_box)
	{
		// TODO: validation
		m_Seed = std::stoi(input_box->GetText()->text);
	}

	void __CreateWorldScene::m_ConfirmCallback(Vivium::Button* button, void* user_params)
	{
		m_Name = m_NameInputBox->GetText()->text;
		m_Seed = std::stoi(m_SeedInputBox->GetText()->text);

		// Get MainMenu instance from params
		MainMenu* main_menu = (MainMenu*)(*(uintptr_t*)user_params);

		// Create instance of game scene
		// NOTE: main menu will take care of freeing this
		__GameScene* game_scene = new __GameScene(m_Seed, m_Name);

		// Start game
		main_menu->m_LoadScene(MainMenu::SceneID::GAME, game_scene);
	}

	__CreateWorldScene::__CreateWorldScene(MainMenu* menu)
	{
		std::size_t __size_unused = 0;

		uintptr_t intaddr0 = reinterpret_cast<uintptr_t>(this);
		uintptr_t intaddr1 = reinterpret_cast<uintptr_t>(menu);

		// TODO: needs some fixing, we're writing the address of the address or something like that
		params = Vivium::Utils::VoidArray::Make(__size_unused, intaddr0, intaddr1);

		m_NameInputBox = MakeRef(Vivium::TextInput,
			Vivium::Quad(0.0f, 300.0f, 300.0f, 100.0f),
			&__CreateWorldScene::s_NameInputCallback,
			(void*)this
		);

		m_SeedInputBox = MakeRef(Vivium::TextInput,
			Vivium::Quad(0.0f, 150.0f, 300.0f, 100.0f),
			&__CreateWorldScene::s_SeedInputCallback,
			(void*)this
		);

		m_ConfirmButton = MakeRef(Vivium::Button,
			Vivium::Quad(0.0f, 0.0f, 300.0f, 100.0f),
			&__CreateWorldScene::s_ConfirmCallback,
			std::string("Create World"),
			params
		);

		Vivium::Application::window_panel->Anchor(Vivium::Panel::ANCHOR::CENTER, Vivium::Panel::ANCHOR::CENTER, m_NameInputBox);
		Vivium::Application::window_panel->Anchor(Vivium::Panel::ANCHOR::CENTER, Vivium::Panel::ANCHOR::CENTER, m_SeedInputBox);
		Vivium::Application::window_panel->Anchor(Vivium::Panel::ANCHOR::CENTER, Vivium::Panel::ANCHOR::CENTER, m_ConfirmButton);
	}

	__CreateWorldScene::~__CreateWorldScene()
	{
		free(params);
	}

	void __CreateWorldScene::Render()
	{
		Vivium::Renderer::Submit(m_NameInputBox.get());
		Vivium::Renderer::Submit(m_SeedInputBox.get());
		Vivium::Renderer::Submit(m_ConfirmButton.get());
	}

	void __CreateWorldScene::Update()
	{
		m_NameInputBox->Update();
		m_SeedInputBox->Update();
		m_ConfirmButton->Update();
	}

	__GameScene::__GameScene(uint32_t world_seed, const std::string& world_name)
	{
		LogInfo("Creating world with seed {}, and name {}", world_seed, world_name);
		m_World = new World(world_seed, world_name);
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

	__LoadWorldScene::VisualWorldSelectable::VisualWorldSelectable(const Vivium::Vector2<float> pos, const std::string& world_name, __LoadWorldScene* world_scene)
		: world_name(world_name)
	{
		panel = MakeRef(Vivium::Panel,
			Vivium::Quad(pos, { 300.0f, 100.0f })
		);

		params = new __CallbackData(world_name, world_scene);

		select_button = MakeRef(Vivium::Button,
			Vivium::Quad(0.0f, { 200.0f, 100.0f }),
			&__LoadWorldScene::s_SelectedWorldCallback,
			world_name,
			(void*)params
		);


		Vivium::Application::window_panel->Anchor(Vivium::Panel::ANCHOR::CENTER, Vivium::Panel::ANCHOR::TOP, panel);
		panel->Anchor(Vivium::Panel::ANCHOR::CENTER, Vivium::Panel::ANCHOR::CENTER, select_button);
	}
	
	__LoadWorldScene::VisualWorldSelectable::~VisualWorldSelectable()
	{
	}

	void __LoadWorldScene::s_SelectedWorldCallback(Vivium::Button* button, void* user_params)
	{
		__CallbackData* callback_data = (__CallbackData*)user_params;
			
		callback_data->scene->m_SelectedWorldCallback(button, user_params);
	}

	void __LoadWorldScene::m_SelectedWorldCallback(Vivium::Button* button, void* user_params)
	{
		__CallbackData* callback_data = (__CallbackData*)user_params;

		std::string world_name = callback_data->world_name;

		// TODO: world seed needs to be stored
		__GameScene* game = new __GameScene(0U, world_name);

		m_Manager->m_LoadScene(MainMenu::SceneID::GAME, game);
	}

	__LoadWorldScene::__LoadWorldScene(MainMenu* menu)
		: m_Manager(menu)
	{
		std::string path = Vivium::Application::resources_path + "saves/";
		Vivium::Vector2<float> current_offset = {0.0f, -100.0f};

		for (auto& it : std::filesystem::directory_iterator(path)) {
			if (it.is_directory()) {
				std::string world_name = it.path().stem().string(); // stem gives us the last part only

				m_Worlds.emplace_back(current_offset, world_name, this);

				LogTrace("Loaded world name: {}", world_name);
				
				// TODO: magic number here
				current_offset.y -= 130.0f;
			}
		}
	}

	__LoadWorldScene::~__LoadWorldScene()
	{
		for (VisualWorldSelectable& selectable : m_Worlds) {
			delete selectable.params;
		}
	}

	void __LoadWorldScene::Render()
	{
		for (VisualWorldSelectable& selectable : m_Worlds) {
			Vivium::Renderer::Submit(selectable.select_button.get());
		}
	}

	void __LoadWorldScene::Update()
	{
		for (VisualWorldSelectable& selectable : m_Worlds) {
			selectable.panel->Update();
			selectable.select_button->Update();
		}
	}

	__LoadWorldScene::__CallbackData::__CallbackData(const std::string& world_name, __LoadWorldScene* scene)
		: world_name(world_name), scene(scene)
	{
	}
}
