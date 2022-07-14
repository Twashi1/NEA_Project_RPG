#pragma once

#include "World.h"
#include "Player.h"

// TODO: should be Engine
class SceneObject {
public:
	enum class Type : uint8_t {
		SHADED_QUAD,
		TEXTURED_QUAD,
		ANIMATION,
		BUTTON,
		TEXT_INPUT,
		TEXT
	};

private:
	struct __shaded_quad { // SHADED_QUAD
		ENG_Ptr(Quad) quad;
		ENG_Ptr(Shader) shader;

		__shaded_quad(ENG_Ptr(Quad) quad, ENG_Ptr(Shader) shader);
	};

	struct __textured_quad { // TEXTURED_QUAD
		ENG_Ptr(Quad) quad;
		ENG_Ptr(Shader) shader;
		ENG_Ptr(Texture) texture;

		__textured_quad(ENG_Ptr(Quad) quad, ENG_Ptr(Shader) shader, ENG_Ptr(Texture) texture);
	};

	union {
		__shaded_quad shaded_quad;
		__textured_quad textured_quad;
		ENG_Ptr(Animation) animation;
		ENG_Ptr(Button) button;
		ENG_Ptr(TextInput) text_input;
		ENG_Ptr(Text) text;
	};

	Type m_Type;

public:
	SceneObject(ENG_Ptr(Quad) quad, ENG_Ptr(Shader) shader, ENG_Ptr(Texture) texture);
	SceneObject(ENG_Ptr(Quad) quad, ENG_Ptr(Shader) shader);
	SceneObject(ENG_Ptr(Animation) animation);
	SceneObject(ENG_Ptr(Button) button);
	SceneObject(ENG_Ptr(TextInput) text_input);
	SceneObject(ENG_Ptr(Text) text);

	void Schedule();
};

class Game {
private:
	static World* m_World;
	static Player m_Player;

	enum class Scene : uint16_t {
		TITLE,
			SAVE_VIEWER,
				WORLD_CREATION,
					WORLD_LOADING,
			OPTIONS,
		GAME
	};

	static Scene m_CurrentScene;

	static void m_RenderTitleScene();
	static void m_RenderGame();

public:
	static void Init();
};