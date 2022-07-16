#include "SceneObject.h"

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

SceneObject::SceneObject(const SceneObject& other)
	: m_Type(other.m_Type)
{
	switch (m_Type) {
	case Type::TEXTURED_QUAD:
		textured_quad = other.textured_quad; break;
	case Type::SHADED_QUAD:
		shaded_quad = other.shaded_quad; break;
	case Type::ANIMATION:
		animation = other.animation; break;
	case Type::BUTTON:
		button = other.button; break;
	case Type::TEXT_INPUT:
		text_input = other.text_input; break;
	case Type::TEXT:
		text = other.text; break;
	default:
		ENG_LogWarn("SceneObject has invalid type {}", to_string(m_Type));
	}
}

SceneObject::~SceneObject()
{
}

SceneObject::Type SceneObject::GetType() const
{
	return m_Type;
}

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
	default:
		ENG_LogWarn("SceneObject has invalid type {}", to_string(m_Type));
	}
}

void SceneObject::Load(Serialiser& s)
{
	// bad code but idgaf
	Deserialise<uint8_t>(s, (uint8_t*)&m_Type);
	
	switch (m_Type) {
	case Type::SHADED_QUAD:
	{
		// TODO: implement
		/*
		shaded_quad.quad = ENG_MakePtr(Quad, s);
		shaded_quad.shader = ENG_MakePtr(Shader, s);
		*/
		break;
	}
	case Type::TEXTURED_QUAD:
	{
		break;
	}
	case Type::ANIMATION:
	{
		break;
	}
	case Type::BUTTON:
	{
		break;
	}
	case Type::TEXT_INPUT:
	{
		break;
	}
	case Type::TEXT:
	{
		break;
	}
	default:
		ENG_LogWarn("SceneObject has invalid type {}", to_string(m_Type));
	}
}

void SceneObject::Unload(Serialiser& s) const
{
	Serialise<uint8_t>(s, (uint8_t)m_Type);
}

SceneObject::__shaded_quad::__shaded_quad(ENG_Ptr(Quad) quad, ENG_Ptr(Shader) shader)
	: quad(quad), shader(shader)
{}

SceneObject::__shaded_quad::__shaded_quad(const __shaded_quad& other)
	: quad(other.quad), shader(other.shader)
{}

SceneObject::__textured_quad::__textured_quad(ENG_Ptr(Quad) quad, ENG_Ptr(Shader) shader, ENG_Ptr(Texture) texture)
	: quad(quad), shader(shader), texture(texture)
{}

SceneObject::__textured_quad::__textured_quad(const __textured_quad& other)
	: quad(other.quad), shader(other.shader), texture(other.texture)
{}

std::string to_string(const SceneObject::Type& type)
{
	switch (type) {
	case SceneObject::Type::ANIMATION:
		return "Animation";
	case SceneObject::Type::BUTTON:
		return "Button";
	case SceneObject::Type::SHADED_QUAD:
		return "Shaded quad";
	case SceneObject::Type::TEXTURED_QUAD:
		return "Textured quad";
	case SceneObject::Type::TEXT_INPUT:
		return "Text input";
	case SceneObject::Type::TEXT:
		return "Text";
	default:
		ENG_LogWarn("SceneObject has invalid type {}", to_string(type)); return "Invalid";
	}
}
