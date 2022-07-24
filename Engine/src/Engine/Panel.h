#pragma once

#include "Utils.h"
#include "Engine.h"
#include "Quad.h"
#include "Shader.h"
#include "Texture.h"

// TODO:
// Panels should be resizeable
// Panels should be moveable
// Panels should be resized with window resize (or at least clamped to window max/min)
// so much logic and code needed for this damn
// end api should be something like

/*
Panel p({200, 200, 300, 300});
Button my_button(some_params); // the x and y given will be the offset from the anchor

p.Anchor(my_button);
or
p.Anchor(<&>my_button.quad); // Should this return the pointer?
*/
class Panel {
public:
	enum ANCHOR : uint8_t {
		LEFT, RIGHT,
		BOTTOM, TOP,
		CENTER
	};

private:
	static std::vector<Panel*> m_Panels; // Store list of all panels, so they can be edited when window is resized

	struct Data {
	private:
		enum class Type : uint8_t {
			QUAD,
			POINT
		};

		Type m_Type;

	public:
		union {
			ENG_Ptr(Quad) quad;
			ENG_Ptr(Vector2<float>) point;
		};
		
		ANCHOR x_anchor, y_anchor;

		Data(ANCHOR x, ANCHOR y, ENG_Ptr(Quad) quad);
		Data(ANCHOR x, ANCHOR y, ENG_Ptr(Vector2<float>) point);

		friend Panel;
	};

	// All quads/points we're editing
	std::vector<Data> m_PanelObjects;

	ENG_Ptr(Quad) m_Quad;

	void m_UpdatePos(Data& panel_object, const Vector2<float>& old_pos, const Vector2<float>& new_pos);
	void m_UpdateDim(Data& panel_object, const Vector2<float>& old_dim, const Vector2<float>& new_dim);
	void m_UpdateQuad(Data& panel_object, const Quad& old_quad, const Quad& new_quad);

public:
	// Set panel coordinates/size
	Panel(ENG_Ptr(Quad) quad);
	~Panel();

	// TODO only called by engine, could be private
	static void UpdateDim(const Vector2<float>& new_dim);

	void SetPos(const Vector2<float>& new_pos);
	void SetDim(const Vector2<float>& new_dim);
	void SetQuad(const Quad& new_quad);

	void Anchor(ANCHOR x, ANCHOR y, Quad& quad);
	void Anchor(ANCHOR x, ANCHOR y, Vector2<float>& point);

	void Anchor(ANCHOR x, ANCHOR y, Quad* quad);
	void Anchor(ANCHOR x, ANCHOR y, Vector2<float>* point);

	void Anchor(ANCHOR x, ANCHOR y, ENG_Ptr(Quad) quad);
	void Anchor(ANCHOR x, ANCHOR y, ENG_Ptr(Vector2<float>) point);

	// For moving/resizing panels (although this requires the panel to be drawn through something)
	void Update();

	// Get quad for debug rendering
	const Quad& GetQuad() const;
};
