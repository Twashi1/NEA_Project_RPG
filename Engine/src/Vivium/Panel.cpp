#include "Panel.h"

namespace Vivium {
	void Panel::Data::SetPos(const Vector2<float>& pos)
	{
		switch (m_Type) {
		case Type::QUAD:
			quad->SetCenter(pos); break;
		case Type::POINT:
		{
			point->x = pos.x; point->y = pos.y;
		} break;
		case Type::PANEL:
			panel->SetPos(pos); break;
		}

		last_pos = pos;
	}

	Vector2<float> Panel::Data::GetPos() const
	{
		Vector2<float> pos;

		switch (m_Type) {
		case Type::QUAD:
			pos = quad->GetCenter(); break;
		case Type::POINT:
			pos = Vector2<float>(point->x, point->y); break;
		case Type::PANEL:
			pos = panel->GetQuad().GetCenter(); break;
		}

		return pos;
	}

	Panel::Data::Data(ANCHOR x, ANCHOR y, Ref(Quad) quad, const Vector2<float>& offset)
		: x_anchor(x), y_anchor(y), quad(quad), m_Type(Type::QUAD), offset(offset)
	{
		if (offset.x == FLT_MAX) {
			this->offset.x = quad->GetX();
		}

		if (offset.y == FLT_MAX) {
			this->offset.y = quad->GetY();
		}

		last_pos = this->offset;
	}

	Panel::Data::Data(ANCHOR x, ANCHOR y, Ref(Vector2<float>) point, const Vector2<float>& offset)
		: x_anchor(x), y_anchor(y), point(point), m_Type(Type::POINT), offset(offset)
	{
		if (offset.x == FLT_MAX) {
			this->offset.x = point->x;
		}

		if (offset.y == FLT_MAX) {
			this->offset.y = point->y;
		}

		last_pos = this->offset;
	}

	Panel::Data::Data(ANCHOR x, ANCHOR y, Ref(Panel) panel, const Vector2<float>& offset)
		: x_anchor(x), y_anchor(y), panel(panel), m_Type(Type::PANEL), offset(offset)
	{
		if (offset.x == FLT_MAX) {
			this->offset.x = panel->GetQuad().GetX();
		}

		if (offset.y == FLT_MAX) {
			this->offset.y = panel->GetQuad().GetY();
		}

		last_pos = this->offset;
	}

	Panel::Data::Data(const Data& other)
		: x_anchor(other.x_anchor), y_anchor(other.y_anchor), m_Type(other.m_Type)
	{
		switch (m_Type) {
		case Type::PANEL: panel = other.panel; break;
		case Type::QUAD: quad = other.quad; break;
		case Type::POINT: point = other.point; break;
		}
	}

	Panel::Data::~Data()
	{}

	Panel::Data& Panel::Data::operator=(const Data& other)
	{
		x_anchor = other.x_anchor; y_anchor = other.y_anchor;
		m_Type = other.m_Type;

		switch (m_Type) {
		case Type::PANEL: panel = other.panel; break;
		case Type::QUAD: quad = other.quad; break;
		case Type::POINT: point = other.point; break;
		}

		return *this;
	}

	void Panel::Update()
	{
		// TODO
	}

	const Quad& Panel::GetQuad() const { return *m_Quad; }

	void Panel::m_Update(Data& panel_object, const Rect& new_rect)
	{
		// Find difference between current and last position to find change in offset, then add to old offset to get new offset
		Vector2<float> current_offset = panel_object.GetPos() - panel_object.last_pos + panel_object.offset;
		// Update new offset
		panel_object.offset = current_offset;

		Vector2<float> new_pos = current_offset;

		// Do calculations to fix to side
		switch (panel_object.x_anchor) {
		case ANCHOR::LEFT:
			new_pos.x += m_Quad->Left(); break;
		case ANCHOR::RIGHT:
			new_pos.x += m_Quad->Right(); break;
		case ANCHOR::CENTER:
			new_pos.x += m_Quad->GetX(); break;
		}

		switch (panel_object.y_anchor) {
		case ANCHOR::BOTTOM:
			new_pos.y += m_Quad->Bottom(); break;
		case ANCHOR::TOP:
			new_pos.y += m_Quad->Top(); break;
		case ANCHOR::CENTER:
			new_pos.y += m_Quad->GetY(); break;
		}

		// Set pos of panel object
		panel_object.SetPos(new_pos);
	}

	Panel::Panel(Ref(Quad) quad)
		: m_Quad(quad), old_pos(quad->GetCenter()), old_dim(quad->GetDim())
	{}

	Panel::Panel(const Quad& quad)
		: m_Quad(MakeRef(Quad, quad)), old_pos(quad.GetCenter()), old_dim(quad.GetDim())
	{}

	Panel::Panel(Quad* quad)
		: m_Quad(Ref(Quad)(quad)), old_pos(quad->GetCenter()), old_dim(quad->GetDim())
	{}

	Panel::~Panel()
	{
		for (Data* data : m_PanelObjects) {
			delete data;
		}
	}

	void Panel::SetPos(const Vector2<float>& new_pos)
	{
		m_Quad->SetCenter(new_pos);

		for (Data* panel_object : m_PanelObjects) {
			m_Update(*panel_object, Rect(new_pos, old_dim));
		}

		old_pos = new_pos;
	}

	void Panel::SetDim(const Vector2<float>& new_dim)
	{
		m_Quad->SetDim(new_dim);

		for (Data* panel_object : m_PanelObjects) {
			m_Update(*panel_object, Rect(old_pos, new_dim));
		}

		old_dim = new_dim;
	}

	void Panel::SetRect(const Rect& new_rect)
	{
		m_Quad->SetRect(new_rect);

		for (Data* panel_object : m_PanelObjects) {
			m_Update(*panel_object, new_rect);
		}

		old_dim = new_rect.dim;
		old_pos = new_rect.center;
	}

	void Panel::Anchor(ANCHOR x, ANCHOR y, Ref(Quad) quad)
	{
		Data* new_data = new Data(x, y, quad);
		m_Update(*new_data, m_Quad->GetRect());
		m_PanelObjects.push_back(new_data);
	}

	void Panel::Anchor(ANCHOR x, ANCHOR y, Ref(Vector2<float>) point)
	{
		Data* new_data = new Data(x, y, point);
		m_Update(*new_data, m_Quad->GetRect());
		m_PanelObjects.push_back(new_data);
	}

	void Panel::Anchor(ANCHOR x, ANCHOR y, Ref(Panel) panel)
	{
		Data* new_data = new Data(x, y, panel);
		m_Update(*new_data, m_Quad->GetRect());
		m_PanelObjects.push_back(new_data);
	}

	void Panel::Anchor(ANCHOR x, ANCHOR y, Ref(Button) button)
	{
		Anchor(x, y, button->quad);
		Anchor(x, y, button->text);
	}

	void Panel::Anchor(ANCHOR x, ANCHOR y, Ref(Slider) slider)
	{
		Anchor(x, y, slider->m_BarQuad);
		Anchor(x, y, slider->m_SliderQuad);
	}

	void Panel::Anchor(ANCHOR x, ANCHOR y, Ref(Text) text)
	{
		Anchor(x, y, Ref(Vector2<float>)(&text->pos));
	}

	void Panel::Anchor(ANCHOR x, ANCHOR y, Ref(TextInput) text_input)
	{
		Anchor(x, y, text_input->m_Text);
		Anchor(x, y, Ref(Quad)(&text_input->quad));
	}

	void Panel::Anchor(ANCHOR x, ANCHOR y, Ref(Sprite) sprite)
	{
		Anchor(x, y, sprite->quad);
	}
}