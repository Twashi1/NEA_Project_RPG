#pragma once

#include "Core.h"
#include "World.h"
#include "Player.h"

namespace Game {
	class MainMenu;

	// TODO other scenes should store MainMenu* manager
	// TODO scene id is basically unused
	// TODO better sharing of data between scenes

	class __StartScene : public Vivium::IScene {
	private:
		using Vivium::IScene::m_SceneID;

		MainMenu* m_Manager = nullptr;

		Ref(Vivium::Sprite) m_TitleSprite;
		Ref(Vivium::Button) m_CreateWorldButton;
		Ref(Vivium::Button) m_LoadWorldButton;

	public:
		__StartScene(int scene_id, MainMenu* menu);
		~__StartScene();

		void Render() override;
		void Update() override;
	};

	// TODO
	class __CreateWorldScene : public Vivium::IScene {
	private:
		using Vivium::IScene::m_SceneID;

		Ref(Vivium::TextInput) m_NameInputBox;
		Ref(Vivium::TextInput) m_SeedInputBox;
		Ref(Vivium::Button) m_ConfirmButton;

		void* params;

		uint32_t m_Seed;
		std::string m_Name;

		static void s_NameInputCallback(Vivium::TextInput* input_box, void* user_params);
		static void s_SeedInputCallback(Vivium::TextInput* input_box, void* user_params);
		static void s_ConfirmCallback(Vivium::Button* button, void* user_params);

		void m_NameInputCallback(Vivium::TextInput* input_box);
		void m_SeedInputCallback(Vivium::TextInput* input_box);
		void m_ConfirmCallback(Vivium::Button* button, void* user_params);

	public:
		__CreateWorldScene(int scene_id, MainMenu* menu);
		~__CreateWorldScene();

		void Render() override;
		void Update() override;
	};

	class __LoadWorldScene : public Vivium::IScene {
	private:
		using Vivium::IScene::m_SceneID;

		// user_params: __LoadWorldScene*, VisualWorldSelectable*
		static void s_SelectedWorldCallback(Vivium::Button* button, void* user_params);
		void m_SelectedWorldCallback(Vivium::Button* button, void* user_params);

		struct __CallbackData {
			std::string world_name;
			__LoadWorldScene* scene;

			__CallbackData(const std::string& world_name, __LoadWorldScene* scene);
		};

		// For the load world scene, it should display all the worlds you have available as small rectangles with the world name, maybe file size, date of creation, etc.
		struct VisualWorldSelectable {
			Ref(Vivium::Panel)	panel;
			Ref(Vivium::Button) select_button;
			std::string			world_name;

			__CallbackData* params;

			// TODO: should take some more world data in future
			VisualWorldSelectable(const Vivium::Vector2<float> pos, const std::string& world_name, __LoadWorldScene* world_scene);
			~VisualWorldSelectable();
		};

		MainMenu* m_Manager = nullptr;

		std::vector<VisualWorldSelectable> m_Worlds;

	public:
		__LoadWorldScene(int scene_id, MainMenu* menu);
		~__LoadWorldScene();

		void Render() override;
		void Update() override;
	};

	// TODO
	class __GameScene : public Vivium::IScene {
	private:
		using Vivium::IScene::m_SceneID;

		World* m_World = nullptr;
		Player* m_Player = nullptr;

	public:
		__GameScene(int scene_id);
		__GameScene(int scene_id, uint32_t world_seed, const std::string& world_name);
		~__GameScene();

		void Render() override;
		void Update() override;
	};

	// TODO: some sort of union or variant for better managing and getting the correct ptr type

	class MainMenu {
	public:
		enum class SceneID : int {
			/*
			Starting screen contains title, button to create new world, and button to load sandbox world
			*/
			START = 0x0062BF01,
			/*
			Create world screen contains text input box to enter name of new world
			*/
			CREATE_WORLD = 0x0062BF02,
			/*
			Loading world screen contains a list of buttons for each potential world to load
			*/
			LOAD_WORLD = 0x0062BF03,
			/*
			Actual game with world player, etc.
			*/
			GAME = 0x0062BF04
		};

	private:
		template <MainMenu::SceneID id>
		struct SceneType { using type = Vivium::IScene; };

		template <> struct SceneType<MainMenu::SceneID::START>			{ using type = __StartScene; };
		template <> struct SceneType<MainMenu::SceneID::CREATE_WORLD>	{ using type = __CreateWorldScene; };
		template <> struct SceneType<MainMenu::SceneID::LOAD_WORLD>		{ using type = __LoadWorldScene; };
		template <> struct SceneType<MainMenu::SceneID::GAME>			{ using type = __GameScene; };

		std::unordered_map<SceneID, Vivium::IScene*> m_Scenes;
		SceneID m_CurrentSceneID;

		static void s_CreateWorldCallback(Vivium::Button* button, void* params);
		void m_CreateWorldCallback(Vivium::Button* button);

		static void s_LoadWorldCallback(Vivium::Button* button, void* params);
		void m_LoadWorldCallback(Vivium::Button* button);

		void m_LoadScene(const SceneID& id);
		void m_LoadScene(const SceneID& id, Vivium::IScene* scene);

		void m_DeallocateUnusedScenes();

	public:
		MainMenu();
		~MainMenu();

		void Update();
		void Render();

		friend __StartScene;
		friend __CreateWorldScene;
		friend __LoadWorldScene;
		friend __GameScene;
	};
}

namespace std {
	template <>
	struct formatter<Game::MainMenu::SceneID> : formatter<string> {
		auto format(Game::MainMenu::SceneID id, format_context& ctx) {
			std::string id_string;

			switch (id) {
			case Game::MainMenu::SceneID::START:		id_string = "Start";		break;
			case Game::MainMenu::SceneID::CREATE_WORLD:	id_string = "Create World";	break;
			case Game::MainMenu::SceneID::GAME:			id_string = "Game";			break;
			default:									id_string = "InvalidID";	break;
			}

			return formatter<string>::format(
				id_string, ctx);
		}
	};
}
