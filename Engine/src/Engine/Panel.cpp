#include "Panel.h"

std::vector<Panel*> Panel::m_Panels{};

Panel::Data::Data(ANCHOR x, ANCHOR y, ENG_Ptr(Quad) quad)
	: x_anchor(x), y_anchor(y), quad(quad), m_Type(Type::QUAD)
{}

Panel::Data::Data(ANCHOR x, ANCHOR y, ENG_Ptr(Vector2<float>) point)
	: x_anchor(x), y_anchor(y), point(point), m_Type(Type::POINT)
{}

void Panel::Update()
{
	// TODO
}

const Quad& Panel::GetQuad() const { return *m_Quad; }

void Panel::m_UpdatePos(Data& panel_object, const Vector2<float>& old_pos, const Vector2<float>& new_pos)
{
	// TODO
}

void Panel::m_UpdateDim(Data& panel_object, const Vector2<float>& old_dim, const Vector2<float>& new_dim)
{
	// TODO
}

void Panel::m_UpdateQuad(Data& panel_object, const Quad& old_quad, const Quad& new_quad)
{
	// TODO
}

Panel::Panel(ENG_Ptr(Quad) quad) : m_Quad(quad)
{
	m_Panels.push_back(this);
}

Panel::~Panel()
{
	Utils::Remove(m_Panels, this);
}

void Panel::UpdateDim(const Vector2<float>& new_dim)
{
	// TODO
}

void Panel::SetPos(const Vector2<float>& new_pos)
{
	Vector2<float> old_pos = m_Quad->GetCenter();

	for (Data& panel_object : m_PanelObjects) {
		m_UpdatePos(panel_object, old_pos, new_pos);
	}

	m_Quad->SetCenter(new_pos);
}

void Panel::SetDim(const Vector2<float>& new_dim)
{
	Vector2<float> old_dim = m_Quad->GetDim();

	for (Data& panel_object : m_PanelObjects) {
		m_UpdateDim(panel_object, old_dim, new_dim);
	}

	m_Quad->SetDim(new_dim);
}

void Panel::SetQuad(const Quad& new_quad)
{
	for (Data& panel_object : m_PanelObjects) {
		m_UpdateQuad(panel_object, *m_Quad, new_quad);
	}
}
