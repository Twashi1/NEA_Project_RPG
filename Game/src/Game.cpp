#include "Game.h"

World* Game::m_World = nullptr;
Game::Scene Game::m_CurrentScene = Game::Scene::TITLE;

void Game::m_RenderTitleScene()
{
}

void Game::m_RenderGame()
{
	m_World->Update({0, 0});
}

void Game::Init()
{
	m_RenderTitleScene();
}

SceneObject::SceneObject(ENG_Ptr(Quad) quad, ENG_Ptr(Shader) shader, ENG_Ptr(Texture) texture)
	: textured_quad(quad, shader, texture), m_Type(Type::TEXTURED_QUAD)
{}

SceneObject::SceneObject(ENG_Ptr(Quad) quad, ENG_Ptr(Shader) shader)
	: shaded_quad(quad, shader), m_Type(Type::SHADED_QUAD)
{}

SceneObject::SceneObject(ENG_Ptr(Animation) animation)
	: animation(animation), m_Type(Type::ANIMATION)
{}

SceneObject::SceneObject(ENG_Ptr(Button) button)
	: button(button), m_Type(Type::BUTTON)
{}

SceneObject::SceneObject(ENG_Ptr(TextInput) text_input)
	: text_input(text_input), m_Type(Type::TEXT_INPUT)
{}

SceneObject::SceneObject(ENG_Ptr(Text) text)
	: text(text), m_Type(Type::TEXT)
{}

void SceneObject::Schedule()
{
	switch (m_Type) {
	case Type::SHADED_QUAD:
		Renderer::Schedule(shaded_quad.quad.get(), shaded_quad.shader.get()); break;
	case Type::TEXTURED_QUAD:
		Renderer::Schedule(textured_quad.quad.get(), textured_quad.shader.get(), textured_quad.texture.get()); break;
	case Type::ANIMATION:
		Renderer::Schedule(animation.get());  break;
	case Type::BUTTON:
		Renderer::Schedule(button.get()); break;
	case Type::TEXT_INPUT:
		Renderer::Schedule(text_input.get()); break;
	case Type::TEXT:
		Renderer::Schedule(text.get()); break;
	}
}

SceneObject::__shaded_quad::__shaded_quad(ENG_Ptr(Quad) quad, ENG_Ptr(Shader) shader)
	: quad(quad), shader(shader)
{}

SceneObject::__textured_quad::__textured_quad(ENG_Ptr(Quad) quad, ENG_Ptr(Shader) shader, ENG_Ptr(Texture) texture)
	: quad(quad), shader(shader), texture(texture)
{}
