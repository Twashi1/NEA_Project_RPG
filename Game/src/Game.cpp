#include "Game.h"

World* Game::m_World = nullptr;
Engine* Game::m_Engine = nullptr;
Game::Scene Game::m_CurrentScene = Game::Scene::TITLE;

void Game::m_RenderTitleScene()
{
}

void Game::Init()
{
	m_Engine = new Engine(1080, 720, 144, true);

	m_RenderTitleScene();
}
