#pragma once

#include "Utils.h"
#include "Quad.h"
#include "Shader.h"
#include "Text.h"
#include "Input.h"

namespace Vivium {
	class ENGINE_API Slider {
	private:
		static ENG_Ptr(Shader) m_DefaultBarShader;
		static ENG_Ptr(Shader) m_DefaultSliderShader;

		float m_Value;

		void m_UpdateValue();
		void m_Construct();

		ENG_Ptr(Quad) m_BarQuad;
		ENG_Ptr(Quad) m_SliderQuad;

		bool isMovingSlider = false;
	public:
		typedef void (*CallbackFunc_t)(Slider*);

		static void Init();

		CallbackFunc_t callback;

		ENG_Ptr(Texture) bar_texture = nullptr;
		ENG_Ptr(Shader) bar_shader;

		ENG_Ptr(Texture) slider_texture = nullptr;
		ENG_Ptr(Shader) slider_shader;

		float GetValue() const;
		float GetValue(float scale) const;
		float GetValue(float min, float max) const;

		void SetValue(float value);
		void SetValue(float value, float scale);
		void SetValue(float value, float min, float max);

		const Vector2<float>& GetPos() const;
		void SetPos(const Vector2<float>& new_pos);

		void Update();

		Slider(ENG_Ptr(Quad) bar_quad, ENG_Ptr(Quad) slider_quad, CallbackFunc_t callback);
		Slider(ENG_Ptr(Quad) bar_quad, ENG_Ptr(Quad) slider_quad, ENG_Ptr(Shader) bar_shader, ENG_Ptr(Shader) slider_shader, CallbackFunc_t callback);
		Slider(ENG_Ptr(Quad) bar_quad, ENG_Ptr(Quad) slider_quad, ENG_Ptr(Shader) bar_shader, ENG_Ptr(Shader) slider_shader, ENG_Ptr(Texture) bar_texture, ENG_Ptr(Texture) slider_texture, CallbackFunc_t callback);
		~Slider();

		friend Renderer;
	};
}
