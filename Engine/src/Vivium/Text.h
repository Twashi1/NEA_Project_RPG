#pragma once

#include "Renderer.h"
#include "Font.h"
#include "Shader.h"
#include "Application.h"
#include "Batch.h"

namespace Vivium {
	class VIVIUM_API Text : IBatchable {
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
			NONE, // TODO: add some validation
			LEFT,
			CENTER,
			RIGHT // TODO: implement
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

		std::string GetText() const;
		void SetText(const std::string& str);

		const Ref(TextureAtlas) GetAtlas() const;
		const Ref(Texture) GetTexture() const;

		void Render() const;
		void Submit(Batch* batch) const override;

		Text();
		Text(const Text& other);
		Text(const std::string& text, const Vector2<float>& pos, const Alignment& alignment = Alignment::LEFT, float scale = 1.0f);
		Text(const std::string& text, const Vector2<float>& pos, Ref(Font) font, const Alignment& alignment = Alignment::LEFT, float scale = 1.0f);
		Text(const std::string& text, const Vector2<float>& pos, Ref(Shader) shader, const Alignment& alignment = Alignment::LEFT, float scale = 1.0f);
		Text(const std::string& text, const Vector2<float>& pos, Ref(Font) font, Ref(Shader) shader, const Alignment& alignment = Alignment::LEFT, float scale = 1.0f);

		~Text();

		friend Application;
	};
}