#include "Game.h"

World* Game::m_World = nullptr;
std::vector<Game::SceneID> Game::m_CurrentScenes{Game::SceneID::TITLE};
std::unordered_map<Game::SceneID, std::vector<SceneObject>> Game::m_Scenes;

void Game::m_UnloadScene(SceneID scene)
{
	switch (scene) {
	case SceneID::TITLE:
		m_UnloadTitle(); break;
	case SceneID::SAVE_VIEWER:
		m_UnloadSaveViewer(); break;
	case SceneID::WORLD_CREATION:
		m_UnloadWorldCreation(); break;
	case SceneID::WORLD_LOADING:
		m_UnloadWorldLoading(); break;
	case SceneID::OPTIONS:
		m_UnloadOptions(); break;
	case SceneID::GAME:
		m_UnloadGame(); break;
	}

	m_Scenes[scene].clear(); m_Scenes[scene].shrink_to_fit();
}

void Game::ShowScene(SceneID scene)
{
	m_LoadScene(scene);
	m_CurrentScenes.push_back(scene);
}

void Game::HideScene(SceneID scene)
{
	m_UnloadScene(scene);
	Utils::Remove(m_CurrentScenes, scene);
}

void Game::Update()
{
	for (auto [id, scene] : m_Scenes) {
		m_UpdateScene(id);
	}
}

void Game::Init()
{
	ShowScene(SceneID::TITLE);
}

void Game::Terminate()
{
	
}

void Game::m_LoadScene(SceneID scene)
{
	switch (scene) {
	case SceneID::TITLE:
		m_LoadTitle(); break;
	case SceneID::SAVE_VIEWER:
		m_LoadSaveViewer(); break;
	case SceneID::WORLD_CREATION:
		m_LoadWorldCreation(); break;
	case SceneID::WORLD_LOADING:
		m_LoadWorldLoading(); break;
	case SceneID::OPTIONS:
		m_LoadOptions(); break;
	case SceneID::GAME:
		m_LoadGame(); break;
	}
}

void Game::m_UpdateScene(SceneID scene)
{
	switch (scene) {
	case SceneID::TITLE:
		m_UpdateTitle(); break;
	case SceneID::SAVE_VIEWER:
		m_UpdateSaveViewer(); break;
	case SceneID::WORLD_CREATION:
		m_UpdateWorldCreation(); break;
	case SceneID::WORLD_LOADING:
		m_UpdateWorldLoading(); break;
	case SceneID::OPTIONS:
		m_UpdateOptions(); break;
	case SceneID::GAME:
		m_UpdateGame(); break;
	}
}
