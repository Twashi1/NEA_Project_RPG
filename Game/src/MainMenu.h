#pragma once

#include "Core.h"
#include "World.h"
#include "Player.h"
#include "WorldMap.h"

// TODO: check if world already has name when creating new world

namespace Game {
	class MainMenu;

	// TODO better sharing of data between scenes

	class __StartScene : public Vivium::IScene {
	private:
		MainMenu* m_Manager = nullptr;

		Ref(Vivium::Sprite) m_TitleSprite;
		Ref(Vivium::Button) m_CreateWorldButton;
		Ref(Vivium::Button) m_LoadWorldButton;

	public:
		__StartScene(MainMenu* menu);
		~__StartScene();

		void Render() override;
		void Update() override;
	};

	// TODO
	class __CreateWorldScene : public Vivium::IScene {
	private:
		Ref(Vivium::TextInput) m_NameInputBox;
		Ref(Vivium::TextInput) m_SeedInputBox;
		Ref(Vivium::Button) m_ConfirmButton;
		Ref(Vivium::Text) m_WorldAlreadyExistsText;
		Ref(Vivium::Shader) m_WorldAlreadyExistsShader;
		float m_WorldAlreadyExistsLifespan = 0.0f;
		Vivium::Timer m_WorldAlreadyExistsTimer;

		static constexpr float s_WorldAlreadyExistsMaxLifespan = 3.0f;
		static constexpr float s_WorldAlreadyExistsFadeout = 0.5f; // Begin fading out 0.5 seconds before the end
		
		Ref(Vivium::Button) m_BackButton; // Takes back to start screen

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
		__CreateWorldScene(MainMenu* menu);
		~__CreateWorldScene();

		void Render() override;
		void Update() override;
	};

	// TODO: Scrolling/scroll bar
	class __LoadWorldScene : public Vivium::IScene {
	private:
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
			Ref(Vivium::Button) select_button;
			std::string			world_name;

			__CallbackData* params;

			// TODO: should take some more world data in future
			VisualWorldSelectable(const Vivium::Vector2<float> pos, const std::string& world_name, __LoadWorldScene* world_scene);
			~VisualWorldSelectable();
		};

		MainMenu* m_Manager = nullptr;

		std::vector<VisualWorldSelectable> m_Worlds;
		
		Ref(Vivium::Panel) m_WorldsPanel; // Panel for the possible worlds to select
		Ref(Vivium::Button) m_BackButton; // Takes back to start screen

	public:
		__LoadWorldScene(MainMenu* menu);
		~__LoadWorldScene();

		void Render() override;
		void Update() override;
	};

	// TODO
	class __GameScene : public Vivium::IScene {
	private:
		World* m_World = nullptr;
		Player* m_Player = nullptr;

	public:
		__GameScene(uint32_t world_seed, const std::string& world_name, bool new_world);
		~__GameScene();

		void Render() override;
		void Update() override;
	};

	// TODO: some sort of union or variant for better managing and getting the correct ptr type

	class MainMenu {
	public:
		enum class SceneID : int {
			START =			0x0062BF01,
			CREATE_WORLD =	0x0062BF02,
			LOAD_WORLD =	0x0062BF03,
			GAME =			0x0062BF04
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

		static void s_BackButtonCallback(Vivium::Button* button, void* params);
		void m_BackButtonCallback(Vivium::Button* button);

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
			case Game::MainMenu::SceneID::LOAD_WORLD:	id_string = "Load World";	break;
			case Game::MainMenu::SceneID::GAME:			id_string = "Game";			break;
			default:									id_string = "InvalidID";	break;
			}

			return formatter<string>::format(
				id_string, ctx);
		}
	};
}
