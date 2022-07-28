#pragma once

#include "Renderer.h"
#include "Font.h"
#include "Shader.h"

namespace Vivium {
	class ENGINE_API Text {
	private:
		static ENG_Ptr(Shader) m_DefaultShader;
		static ENG_Ptr(Font) m_DefaultFont;

		void m_Construct();

	public:
		std::string text;			// Text to render
		Vector2<float> pos;			// Position to render text
		ENG_Ptr(Font) font;			// Pointer to font
		ENG_Ptr(Shader) shader;		// Shader for text
		float scale;				// Scale for text

		static void Init();
		static ENG_Ptr(Font) GetDefaultFont();

		// TODO might need fixes
		void SetPos(const Vector2<float>& pos);
		const Vector2<float>& GetPos() const;

		Text();
		Text(const Text& other);
		Text(const std::string& text, const Vector2<float>& pos, float scale = 1.0f);
		Text(const std::string& text, const Vector2<float>& pos, ENG_Ptr(Font) font, float scale = 1.0f);
		Text(const std::string& text, const Vector2<float>& pos, ENG_Ptr(Shader) shader, float scale = 1.0f);
		Text(const std::string& text, const Vector2<float>& pos, ENG_Ptr(Font) font, ENG_Ptr(Shader) shader, float scale = 1.0f);

		~Text();
	};
}