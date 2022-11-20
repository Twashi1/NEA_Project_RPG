#pragma once

#include "Core.h"
#include "World.h"
#include "Player.h"
#include "WorldMap.h"

namespace Game {
	class SceneManager;

	// TODO: better sharing of data between scenes

	/// <summary>
	/// The title screen displaying a background and the buttons for creating world,
	/// loading world, and changing options
	/// </summary>
	class StartScene : public Vivium::IScene {
	private:
		SceneManager* m_Manager = nullptr;

		// Sprite for the title
		std::shared_ptr<Vivium::Sprite> m_TitleSprite;
		// Buttons
		std::shared_ptr<Vivium::Button> m_CreateWorldButton;
		std::shared_ptr<Vivium::Button> m_LoadWorldButton;
		std::shared_ptr<Vivium::Button> m_OptionsButton;

		// Background texture, quad, and shader
		std::shared_ptr<Vivium::Texture> m_BackgroundTexture;
		std::shared_ptr<Vivium::Quad>	 m_BackgroundQuad;
		std::shared_ptr<Vivium::Shader>  m_BackgroundShader;

	public:
		StartScene(SceneManager* menu);
		virtual ~StartScene();

		virtual void Render() override;
		virtual void Update() override;
	};

	/// <summary>
	/// Creating world screen, displaying boxes to enter information about the world,
	/// error text if the user typed a world that already exists, and a confirmation box
	/// </summary>
	class CreateWorldScene : public Vivium::IScene {
	private:
		// Entry boxes for name and seed
		std::shared_ptr<Vivium::TextInput> m_NameInputBox;
		std::shared_ptr<Vivium::TextInput> m_SeedInputBox;
		// Confirmation button to start game
		std::shared_ptr<Vivium::Button> m_ConfirmButton;
		// Text and shader for displaying visual feedback that world exists/doesn't
		std::shared_ptr<Vivium::Text> m_WorldAlreadyExistsText;
		std::shared_ptr<Vivium::Shader> m_WorldAlreadyExistsShader;
		// How long the error text has been displayed for
		float m_WorldAlreadyExistsLifespan = 0.0f;
		Vivium::Timer m_WorldAlreadyExistsTimer;

		// World already exists display text lasts for 3 seconds, and starts fading out after 2.5s
		static constexpr float s_WorldAlreadyExistsMaxLifespan = 3.0f;
		static constexpr float s_WorldAlreadyExistsFadeout = 0.5f; // Begin fading out 0.5 seconds before the end
		
		std::shared_ptr<Vivium::Button> m_BackButton; // Takes back to start screen

		// TODO: what is this
		void* params;

		// World seed
		uint32_t m_Seed;
		// World name
		std::string m_Name;

		static void s_ConfirmCallback(Vivium::Button* button, void* user_params);

		void m_ConfirmCallback(Vivium::Button* button, void* user_params);

	public:
		CreateWorldScene(SceneManager* menu);
		virtual ~CreateWorldScene();

		virtual void Render() override;
		virtual void Update() override;
	};

	/// <summary>
	/// Displays various options the player can change before getting into game
	/// </summary>
	class OptionsScene : public Vivium::IScene {
	private:
		SceneManager* m_Manager;

		static void s_FPSCallback(Vivium::Slider* slider, void* user_params);
		static void s_VolumeCallback(Vivium::Slider* slider, void* user_params);

		static constexpr int FPS_MIN = 60;
		static constexpr int FPS_MAX = 240;

		// Options panel
		std::shared_ptr<Vivium::Panel> m_OptionsPanel;

		// GUI items for controlling framerate
		std::shared_ptr<Vivium::Slider> m_FPSSlider;
		std::shared_ptr<Vivium::Text> m_FPSText;

		// GUI items for controlling sound volume
		std::shared_ptr<Vivium::Slider> m_VolumeSlider;
		std::shared_ptr<Vivium::Text> m_VolumeText;

		// Back to start screen
		std::shared_ptr<Vivium::Button> m_BackButton;

	public:
		OptionsScene(SceneManager* manager);
		virtual ~OptionsScene() = default;
		
		virtual void Render() override;
		virtual void Update() override;
	};

	/// <summary>
	/// Displays list of existing worlds to select from
	/// </summary>
	class LoadWorldScene : public Vivium::IScene {
	private:
		// user_params: LoadWorldScene*, VisualWorldSelectable*
		static void s_SelectedWorldCallback(Vivium::Button* button, void* user_params);
		void m_SelectedWorldCallback(Vivium::Button* button, void* user_params);

		struct __CallbackData {
			std::string world_name;
			LoadWorldScene* scene;

			__CallbackData(const std::string& world_name, LoadWorldScene* scene);
		};

		// For the load world scene, it should display all the worlds you have available as small rectangles with the world name, maybe file size, date of creation, etc.
		// TODO: size, and creation data
		struct VisualWorldSelectable {
			// TODO: unique?
			std::shared_ptr<Vivium::Button> select_button;
			std::string	world_name;

			__CallbackData* params;

			// TODO: should take some more world data in future
			VisualWorldSelectable(const Vivium::Vector2<float> pos, const std::string& world_name, LoadWorldScene* world_scene);
			~VisualWorldSelectable();
		};

		SceneManager* m_Manager = nullptr;

		// List of possible worlds to select from
		std::vector<VisualWorldSelectable> m_Worlds;
		
		std::shared_ptr<Vivium::Panel> m_WorldsPanel; // Panel for the possible worlds to select
		std::shared_ptr<Vivium::Button> m_BackButton; // Takes back to start screen

	public:
		LoadWorldScene(SceneManager* menu);
		virtual ~LoadWorldScene();

		virtual void Render() override;
		virtual void Update() override;
	};

	/// <summary>
	/// The actual game itself, with the player, world, etc.
	/// </summary>
	class GameScene : public Vivium::IScene {
	private:
		World* m_World = nullptr;
		Player* m_Player = nullptr;

	public:
		GameScene(uint32_t world_seed, const std::string& world_name, bool new_world);
		virtual ~GameScene();

		virtual void Render() override;
		virtual void Update() override;
	};

	/// <summary>
	/// Managing the scene that should be displayed, also storing the scene
	/// </summary>
	class SceneManager {
	public:
		enum class SceneID : int {
			START =			0x0062BF01,
			CREATE_WORLD =	0x0062BF02,
			LOAD_WORLD =	0x0062BF03,
			GAME =			0x0062BF04,
			OPTIONS =		0x0062BF05
		};

	private:
		template <SceneManager::SceneID id>
		struct SceneType { using type = Vivium::IScene; };

		template <> struct SceneType<SceneManager::SceneID::START>			{ using type = StartScene; };
		template <> struct SceneType<SceneManager::SceneID::CREATE_WORLD>	{ using type = CreateWorldScene; };
		template <> struct SceneType<SceneManager::SceneID::LOAD_WORLD>		{ using type = LoadWorldScene; };
		template <> struct SceneType<SceneManager::SceneID::GAME>			{ using type = GameScene; };
		template <> struct SceneType<SceneManager::SceneID::OPTIONS>		{ using type = OptionsScene; };

		std::unordered_map<SceneID, Vivium::IScene*> m_Scenes;
		SceneID m_CurrentSceneID;

		static void s_BackButtonCallback(Vivium::Button* button, void* params);
		void m_BackButtonCallback(Vivium::Button* button);

		static void s_CreateWorldCallback(Vivium::Button* button, void* params);
		void m_CreateWorldCallback(Vivium::Button* button);

		static void s_LoadWorldCallback(Vivium::Button* button, void* params);
		void m_LoadWorldCallback(Vivium::Button* button);

		static void s_OptionsCallback(Vivium::Button* button, void* params);
		void m_OptionsCallback(Vivium::Button* button);

		void m_LoadScene(const SceneID& id);
		void m_LoadScene(const SceneID& id, Vivium::IScene* scene);

		void m_DeallocateUnusedScenes();

	public:
		SceneManager();
		~SceneManager();

		void Update();
		void Render();

		friend StartScene;
		friend CreateWorldScene;
		friend LoadWorldScene;
		friend GameScene;
		friend OptionsScene;
	};
}

namespace std {
	// Formatter debug
	template <>
	struct formatter<Game::SceneManager::SceneID> : formatter<string> {
		auto format(Game::SceneManager::SceneID id, format_context& ctx) {
			std::string id_string;

			switch (id) {
			case Game::SceneManager::SceneID::START:		id_string = "Start";		break;
			case Game::SceneManager::SceneID::CREATE_WORLD:	id_string = "Create World";	break;
			case Game::SceneManager::SceneID::LOAD_WORLD:	id_string = "Load World";	break;
			case Game::SceneManager::SceneID::GAME:			id_string = "Game";			break;
			default:									id_string = "InvalidID";	break;
			}

			return formatter<string>::format(
				id_string, ctx);
		}
	};
}
