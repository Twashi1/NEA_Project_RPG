#pragma once

#include "Logger.h"
#include "Quad.h"
#include "Shader.h"
#include "Text.h"
#include "Input.h"
#include "Application.h"

namespace Vivium {
	class VIVIUM_API Slider {
	public:
		static Ref(Shader) m_DefaultBarShader;
		static Ref(Shader) m_DefaultSliderShader;

		float m_Value;

		void m_UpdateValue();
		void m_Construct();

		Ref(Quad) m_BarQuad;
		Ref(Quad) m_SliderQuad;

		bool isMovingSlider = false;

		void* m_UserParams = nullptr;

		static void m_Init();

	public:
		typedef void (*CallbackFunc_t)(Slider*, void*);

		CallbackFunc_t callback;

		Ref(Texture) bar_texture = nullptr;
		Ref(Shader) bar_shader;

		Ref(Texture) slider_texture = nullptr;
		Ref(Shader) slider_shader;

		float GetValue() const;
		float GetValue(float scale) const;
		float GetValue(float min, float max) const;

		void SetValue(float value);
		void SetValue(float value, float scale);
		void SetValue(float value, float min, float max);

		Vector2<float> GetPos() const;
		void SetPos(const Vector2<float>& new_pos);

		void SetUserParams(void* userParams) { m_UserParams = userParams; }

		void Update();

		Slider(Ref(Quad) bar_quad, Ref(Quad) slider_quad, CallbackFunc_t callback, void* userParams = nullptr);
		Slider(Ref(Quad) bar_quad, Ref(Quad) slider_quad, Ref(Shader) bar_shader, Ref(Shader) slider_shader, CallbackFunc_t callback, void* userParams = nullptr);
		Slider(Ref(Quad) bar_quad, Ref(Quad) slider_quad, Ref(Shader) bar_shader, Ref(Shader) slider_shader, Ref(Texture) bar_texture, Ref(Texture) slider_texture, CallbackFunc_t callback, void* userParams = nullptr);
		~Slider();

		friend Renderer;
		friend Application;
	};
}
