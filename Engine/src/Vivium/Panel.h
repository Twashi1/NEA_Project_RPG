#pragma once

#include "Logger.h"
#include "Quad.h"
#include "Slider.h"
#include "Button.h"
#include "TextInput.h"
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
	class VIVIUM_API Panel {
	public:
		enum class VIVIUM_API ANCHOR : uint8_t {
			LEFT, RIGHT,
			BOTTOM, TOP,
			CENTER
		};

	private:
		struct VIVIUM_API Data {
		private:
			enum class VIVIUM_API Type : uint8_t {
				PANEL,
				QUAD,
				POINT
			};

			Type m_Type;

		public:
			union {
				Ref(Panel) panel;
				Ref(Quad) quad;
				Ref(Vector2<float>) point;
			};

			Vector2<float> last_pos;
			Vector2<float> offset;

			// TODO implement for slider, button, etc.
			void SetPos(const Vector2<float>& pos);
			Vector2<float> GetPos() const;

			// Assuming object passed has position relative to anchor coordinates
			Data(ANCHOR x, ANCHOR y, Ref(Quad) quad, const Vector2<float>& offset = Vector2<float>(FLT_MAX, FLT_MAX));
			Data(ANCHOR x, ANCHOR y, Ref(Vector2<float>) point, const Vector2<float>& offset = Vector2<float>(FLT_MAX, FLT_MAX));
			Data(ANCHOR x, ANCHOR y, Ref(Panel) panel, const Vector2<float>& offset = Vector2<float>(FLT_MAX, FLT_MAX));
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
		Ref(Quad) m_Quad;

		void m_Update(Data& panel_object, const Rect& new_rect);

	public:
		// Set panel coordinates/size
		Panel(Ref(Quad) quad);
		Panel(const Quad& quad);
		Panel(Quad* quad);
		~Panel();

		void SetPos(const Vector2<float>& new_pos);
		void SetDim(const Vector2<float>& new_dim);
		void SetRect(const Rect& new_rect);

		void Anchor(ANCHOR x, ANCHOR y, Ref(Quad) quad);
		void Anchor(ANCHOR x, ANCHOR y, Ref(Vector2<float>) point);
		void Anchor(ANCHOR x, ANCHOR y, Ref(Panel) panel);

		void Anchor(ANCHOR x, ANCHOR y, Ref(Button) button);
		void Anchor(ANCHOR x, ANCHOR y, Ref(Slider) slider);
		void Anchor(ANCHOR x, ANCHOR y, Ref(TextInput) text_input);
		void Anchor(ANCHOR x, ANCHOR y, Ref(Text) text);

		// For moving/resizing panels (although this requires the panel to be drawn through something)
		void Update();

		// Get quad for debug rendering
		const Quad& GetQuad() const;
	};
}