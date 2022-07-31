#pragma once

#include "Renderer.h"
#include "Font.h"
#include "Shader.h"

namespace Vivium {
	class VIVIUM_API Text {
	private:
		static Ref(Shader) m_DefaultShader;
		static Ref(Font) m_DefaultFont;

		void m_Construct();

	public:
		std::string text;			// Text to render
		Vector2<float> pos;			// Position to render text
		Ref(Font) font;			// Pointer to font
		Ref(Shader) shader;		// Shader for text
		float scale;				// Scale for text

		static void Init();
		static Ref(Font) GetDefaultFont();

		// TODO might need fixes
		void SetPos(const Vector2<float>& pos);
		const Vector2<float>& GetPos() const;

		Text();
		Text(const Text& other);
		Text(const std::string& text, const Vector2<float>& pos, float scale = 1.0f);
		Text(const std::string& text, const Vector2<float>& pos, Ref(Font) font, float scale = 1.0f);
		Text(const std::string& text, const Vector2<float>& pos, Ref(Shader) shader, float scale = 1.0f);
		Text(const std::string& text, const Vector2<float>& pos, Ref(Font) font, Ref(Shader) shader, float scale = 1.0f);

		~Text();
	};
}