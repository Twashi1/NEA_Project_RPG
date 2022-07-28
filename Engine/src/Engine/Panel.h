#pragma once

#include "Utils.h"
#include "Quad.h"
#include "Slider.h"
#include "Button.h"
#include "TextInput.h"
#include "ToggleSwitch.h"
#include "Text.h"

/*
Panel p({200, 200, 300, 300});
Button my_button(some_params); // the x and y given will be the offset from the anchor

Application::window_panel->Anchor(ANCHOR::RIGHT, ANCHOR::TOP, &p);

p.Anchor(my_button); // This will edit multiple parameters in button class, since both text and quad has to be moved
or
p.Anchor(quad_ptr);
*/
namespace Vivium {
	class ENGINE_API Panel {
	public:
		enum class ENGINE_API ANCHOR : uint8_t {
			LEFT, RIGHT,
			BOTTOM, TOP,
			CENTER
		};

	private:
		struct ENGINE_API Data {
		private:
			enum class ENGINE_API Type : uint8_t {
				PANEL,
				QUAD,
				POINT,
				BUTTON,
				SLIDER,
				TEXT_INPUT,
				TOGGLE_SWITCH,
				TEXT
			};

			Type m_Type;

		public:
			union {
				ENG_Ptr(Panel) panel;
				ENG_Ptr(Quad) quad;
				ENG_Ptr(Vector2<float>) point;
				ENG_Ptr(Slider) slider;
				ENG_Ptr(Button) button;
				ENG_Ptr(TextInput) text_input;
				ENG_Ptr(ToggleSwitch) toggle_switch;
				ENG_Ptr(Text) text;
			};

			Vector2<float> last_pos;
			Vector2<float> offset;

			// TODO implement for slider, button, etc.
			void SetPos(const Vector2<float>& pos);
			Vector2<float> GetPos() const;

			// Assuming object passed has position relative to anchor coordinates
			Data(ANCHOR x, ANCHOR y, ENG_Ptr(Quad) quad, const Vector2<float>& offset = Vector2<float>(FLT_MAX, FLT_MAX));
			Data(ANCHOR x, ANCHOR y, ENG_Ptr(Vector2<float>) point, const Vector2<float>& offset = Vector2<float>(FLT_MAX, FLT_MAX));
			Data(ANCHOR x, ANCHOR y, ENG_Ptr(Panel) panel, const Vector2<float>& offset = Vector2<float>(FLT_MAX, FLT_MAX));
			Data(ANCHOR x, ANCHOR y, ENG_Ptr(Slider) slider, const Vector2<float>& offset = Vector2<float>(FLT_MAX, FLT_MAX));
			Data(ANCHOR x, ANCHOR y, ENG_Ptr(Button) button, const Vector2<float>& offset = Vector2<float>(FLT_MAX, FLT_MAX));
			Data(ANCHOR x, ANCHOR y, ENG_Ptr(TextInput) text_input, const Vector2<float>& offset = Vector2<float>(FLT_MAX, FLT_MAX));
			Data(ANCHOR x, ANCHOR y, ENG_Ptr(ToggleSwitch) toggle_switch, const Vector2<float>& offset = Vector2<float>(FLT_MAX, FLT_MAX));
			Data(ANCHOR x, ANCHOR y, ENG_Ptr(Text) text, const Vector2<float>& offset = Vector2<float>(FLT_MAX, FLT_MAX));
			Data(const Data& other);
			~Data();

			Data& operator=(const Data& other);

			ANCHOR x_anchor, y_anchor;

			friend Panel;
		};

		// All quads/points we're editing
		std::vector<Data*> m_PanelObjects;

		Vector2<float> old_dim;
		Vector2<float> old_pos;
		ENG_Ptr(Quad) m_Quad;

		void m_Update(Data& panel_object, const Rect& new_rect);

	public:
		// Set panel coordinates/size
		Panel(ENG_Ptr(Quad) quad);
		Panel(const Quad& quad);
		Panel(Quad* quad);
		~Panel();

		void SetPos(const Vector2<float>& new_pos);
		void SetDim(const Vector2<float>& new_dim);
		void SetRect(const Rect& new_rect);

		void Anchor(ANCHOR x, ANCHOR y, ENG_Ptr(Quad) quad);
		void Anchor(ANCHOR x, ANCHOR y, ENG_Ptr(Vector2<float>) point);
		void Anchor(ANCHOR x, ANCHOR y, ENG_Ptr(Panel) panel);

		void Anchor(ANCHOR x, ANCHOR y, ENG_Ptr(Button) button);
		void Anchor(ANCHOR x, ANCHOR y, ENG_Ptr(Slider) slider);
		void Anchor(ANCHOR x, ANCHOR y, ENG_Ptr(TextInput) text_input);
		void Anchor(ANCHOR x, ANCHOR y, ENG_Ptr(ToggleSwitch) toggle_switch);
		void Anchor(ANCHOR x, ANCHOR y, ENG_Ptr(Text) text);

		// For moving/resizing panels (although this requires the panel to be drawn through something)
		void Update();

		// Get quad for debug rendering
		const Quad& GetQuad() const;
	};
}