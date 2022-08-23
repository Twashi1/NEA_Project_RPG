#pragma once

#include "Core.h"
#include "World.h"
#include "Player.h"

namespace Game {
	class MainMenu;

	class __StartScene : public Vivium::IScene {
	private:
		using Vivium::IScene::m_SceneID;

		Ref(Vivium::Sprite) m_TitleSprite;
		Ref(Vivium::Button) m_StartGameButton;

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

	public:
		__CreateWorldScene(int scene_id);
		~__CreateWorldScene();

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
			Actual game with world player, etc.
			*/
			GAME = 0x0062BF03
		};

	private:
		template <MainMenu::SceneID id>
		struct SceneType { using type = Vivium::IScene; };

		template <> struct SceneType<MainMenu::SceneID::START>			{ using type = __StartScene; };
		template <> struct SceneType<MainMenu::SceneID::CREATE_WORLD>	{ using type = __CreateWorldScene; };
		template <> struct SceneType<MainMenu::SceneID::GAME>			{ using type = __GameScene; };

		std::unordered_map<SceneID, Vivium::IScene*> m_Scenes;
		SceneID m_CurrentSceneID;

		static void m_StartGameCallback(Vivium::Button* button, void* params);
		void m_StartGame(Vivium::Button* button);

		void m_LoadScene(const SceneID& id);

		void m_DeallocateUnusedScenes();

	public:
		MainMenu();
		~MainMenu();

		void Update();
		void Render();

		friend __StartScene;
		friend __CreateWorldScene;
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
