#pragma once

#include "World.h"
#include "Player.h"

class Game {
public:
	enum class SceneID : uint16_t {
		TITLE,
		SAVE_VIEWER,
		WORLD_CREATION,
		WORLD_LOADING,
		OPTIONS,
		GAME
	};

private:
	static World* m_World;
	static Player m_Player;

	static std::unordered_map<SceneID, std::vector<SceneObject>> m_Scenes;

	static std::vector<SceneID> m_CurrentScenes;

	static void m_LoadScene(SceneID scene);
	static void m_LoadTitle() {};
	static void m_LoadSaveViewer() {};
	static void m_LoadWorldCreation() {};
	static void m_LoadWorldLoading() {};
	static void m_LoadOptions() {};
	static void m_LoadGame() {};

	static void m_UnloadScene(SceneID scene);
	static void m_UnloadTitle() {};
	static void m_UnloadSaveViewer() {};
	static void m_UnloadWorldCreation() {};
	static void m_UnloadWorldLoading() {};
	static void m_UnloadOptions() {};
	static void m_UnloadGame() {};

	static void m_UpdateScene(SceneID scene);
	static void m_UpdateTitle() {};
	static void m_UpdateSaveViewer() {};
	static void m_UpdateWorldCreation() {};
	static void m_UpdateWorldLoading() {};
	static void m_UpdateOptions() {};
	static void m_UpdateGame() {};


public:
	static void ShowScene(SceneID scene);
	static void HideScene(SceneID scene);

	static void Update();
	static void Init();
	static void Terminate();
};