#pragma once

#include "Utils.h"
#include "Quad.h"
#include "Shader.h"
#include "Texture.h"
#include "Text.h"
#include "TextInput.h"
#include "Animation.h"
#include "Serialiser.h"

struct ENGINE_API SceneObject/* : public Serialiseable*/ {
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
		__shaded_quad(const __shaded_quad& other);
		~__shaded_quad() = default;
	};

	struct __textured_quad { // TEXTURED_QUAD
		ENG_Ptr(Quad) quad;
		ENG_Ptr(Shader) shader;
		ENG_Ptr(Texture) texture;

		__textured_quad(ENG_Ptr(Quad) quad, ENG_Ptr(Shader) shader, ENG_Ptr(Texture) texture);
		__textured_quad(const __textured_quad& other);
		~__textured_quad() = default;
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

	SceneObject(const SceneObject& other);
	~SceneObject();

	Type GetType() const;
	void Schedule();

	/*void Load(Serialiser& s) override;
	void Unload(Serialiser& s) const override;*/
};

std::string ENGINE_API to_string(const SceneObject::Type& type);