#pragma once

#include "Renderer.h"
#include "Font.h"
#include "Shader.h"
#include "Application.h"

namespace Vivium {
	class VIVIUM_API Text {
	private:
		static Ref(Shader) m_DefaultShader;
		static Ref(Font) m_DefaultFont;

		void m_Construct(const std::string& input_str);

		static void m_Init();

		float m_GetWidth(const std::string& str) const;

		Ref(Texture) m_FontTexture;
		Ref(TextureAtlas) m_FontTextureAtlas;

	public:
		enum class Alignment : uint8_t {
			NONE,
			LEFT,
			MIDDLE // Not adding right alignment because not gonna use it
		};

		Alignment alignment;
		std::vector<std::string> strings;				// Strings to render, split by new lines
		Ref(Vector2<float>) pos;						// Position to render text
		Ref(Font) font;									// Pointer to font
		Ref(Shader) shader;								// Shader for text
		float scale;									// Scale for text

		static Ref(Font) GetDefaultFont();

		// TODO might need fixes
		void SetPos(const Vector2<float>& pos);
		Vector2<float> GetPos() const;

		void FixToWidth(float max_width);

		float GetWidth() const;
		float GetHeight() const;

		void Render() const;

		Text();
		Text(const Text& other);
		Text(const std::string& text, const Vector2<float>& pos, float scale = 1.0f);
		Text(const std::string& text, const Vector2<float>& pos, Ref(Font) font, float scale = 1.0f);
		Text(const std::string& text, const Vector2<float>& pos, Ref(Shader) shader, float scale = 1.0f);
		Text(const std::string& text, const Vector2<float>& pos, Ref(Font) font, Ref(Shader) shader, float scale = 1.0f);

		~Text();

		friend Application;
	};
}