#include "Slider.h"

ENG_Ptr(Shader) Slider::m_DefaultBarShader = nullptr;
ENG_Ptr(Shader) Slider::m_DefaultSliderShader = nullptr;

void Slider::m_UpdateValue()
{
	m_Value = (m_SliderQuad->GetX() - m_BarQuad->Left()) / m_BarQuad->GetWidth();
}

void Slider::m_Construct()
{
	// Calculate initial value from quad position
	float initial = (m_SliderQuad->GetX() - m_BarQuad->Left()) / m_BarQuad->GetWidth();
	// Clamp it from 0 to 1 and set m_Value
	SetValue(std::min(std::max(initial, 0.0f), 1.0f));
	// Update GUI manager
	GUIManager::sliders.push_back(this);
}

void Slider::Init()
{
	m_DefaultBarShader = ENG_MakePtr(Shader, "static_texture_vertex", "slider_frag");
	m_DefaultBarShader->SetUniform3f("u_BarColor", COLORS::GRAY);
	m_DefaultBarShader->SetUniform3f("u_ShadedColor", COLORS::DARKGRAY);
	m_DefaultSliderShader = ENG_MakePtr(Shader, "static_texture_vertex", "color_frag");
	m_DefaultSliderShader->SetUniform3f("u_Color", COLORS::DARKGRAY);
}

float Slider::GetValue() const
{
	return m_Value;
}

float Slider::GetValue(float scale) const
{
	return m_Value * scale;
}

float Slider::GetValue(float min, float max) const
{
	return (m_Value * (max - min)) + min;
}

void Slider::SetValue(float value)
{
	// TODO lots of repeated code here
	m_Value = value;
	float bar_left = m_BarQuad->Left();
	m_SliderQuad->SetX(bar_left + m_BarQuad->GetWidth() * value);
}

void Slider::SetValue(float value, float scale)
{
	m_Value = value / scale;
	float bar_left = m_BarQuad->Left();
	m_SliderQuad->SetX(bar_left + m_BarQuad->GetWidth() * m_Value);
}

void Slider::SetValue(float value, float min, float max)
{
	m_Value = (value - min) / (max - min);
	float bar_left = m_BarQuad->Left();
	m_SliderQuad->SetX(bar_left + m_BarQuad->GetWidth() * m_Value);
}

void Slider::Update(const Vector2<float>& cursor_pos, Input::State lmb_state)
{
	// If our cursor is above the slider
	if (isMovingSlider || m_BarQuad->Contains(cursor_pos)) {
		isMovingSlider = true;

		// If pressing or holding slider
		if (lmb_state == Input::State::PRESS || lmb_state == Input::State::HOLD) {
			float new_pos = std::min(std::max(cursor_pos.x, m_BarQuad->Left()), m_BarQuad->Right());

			m_SliderQuad->SetX(new_pos);
			// Update value
			m_UpdateValue();
			// Run callback
			callback(this);
		}
		else {
			isMovingSlider = false;
		}
	}

	bar_shader->Bind();
	bar_shader->SetUniform1f("u_Value", m_Value);
}

Slider::Slider(ENG_Ptr(Quad) bar_quad, ENG_Ptr(Quad) slider_quad, CallbackFunc_t callback)
	: m_Value(0.0f), m_BarQuad(bar_quad), m_SliderQuad(slider_quad), callback(callback), bar_shader(m_DefaultBarShader), slider_shader(m_DefaultSliderShader)
{
	m_Construct();
}

Slider::Slider(ENG_Ptr(Quad) bar_quad, ENG_Ptr(Quad) slider_quad, ENG_Ptr(Shader) bar_shader, ENG_Ptr(Shader) slider_shader, CallbackFunc_t callback)
	: m_Value(0.0f), m_BarQuad(bar_quad), m_SliderQuad(slider_quad), callback(callback), bar_shader(bar_shader), slider_shader(slider_shader)
{
	m_Construct();
}

Slider::Slider(ENG_Ptr(Quad) bar_quad, ENG_Ptr(Quad) slider_quad, ENG_Ptr(Shader) bar_shader, ENG_Ptr(Shader) slider_shader, ENG_Ptr(Texture) bar_texture, ENG_Ptr(Texture) slider_texture, CallbackFunc_t callback)
	: m_Value(0.0f), m_BarQuad(bar_quad), m_SliderQuad(slider_quad), callback(callback), bar_shader(bar_shader), slider_shader(slider_shader), bar_texture(bar_texture), slider_texture(slider_texture)
{
	m_Construct();
}

Slider::~Slider()
{
	Utils::Remove(GUIManager::sliders, this);
}
