#include "Quad.h"

void Quad::m_UpdateVB()
{
	m_Rect.m_UpdateTrig();

	std::vector<Vector2<float>> vertices = m_Rect.GetVertices();

	// Set vertex buffer vertices
	vb->Set(
		{
			vertices[Rect::BOTTOMLEFT ].x, vertices[Rect::BOTTOMLEFT ].y,
			vertices[Rect::BOTTOMRIGHT].x, vertices[Rect::BOTTOMRIGHT].y,
			vertices[Rect::TOPRIGHT   ].x, vertices[Rect::TOPRIGHT   ].y,
			vertices[Rect::TOPLEFT    ].x, vertices[Rect::TOPLEFT    ].y
		}
	);
}

void Quad::m_ConstructBuffers()
{
	m_Rect.m_UpdateTrig();

	std::vector<Vector2<float>> vertices = m_Rect.GetVertices();

	// Generate vertex and texture coordinates
	vb = std::shared_ptr<VertexBuffer>(new VertexBuffer(
		{
			vertices[Rect::BOTTOMLEFT ].x, vertices[Rect::BOTTOMLEFT ].y,
			vertices[Rect::BOTTOMRIGHT].x, vertices[Rect::BOTTOMRIGHT].y,
			vertices[Rect::TOPRIGHT   ].x, vertices[Rect::TOPRIGHT	 ].y,
			vertices[Rect::TOPLEFT	  ].x, vertices[Rect::TOPLEFT	 ].y
		},
		{
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f
		}
	));

	// Setup index buffer
	ib = std::shared_ptr<IndexBuffer>(new IndexBuffer({ 0, 1, 2, 2, 3, 0 }));
}

void Quad::SetCenter(float nx, float ny)
{
	m_Rect.SetCenter(nx, ny);
	m_UpdateVB();
}

void Quad::SetCenter(const Vector2<float>& ncenter)
{
	m_Rect.SetCenter(ncenter);
	m_UpdateVB();
}

void Quad::SetDim(float nwidth, float nheight)
{
	m_Rect.SetDim(nwidth, nheight);
	m_UpdateVB();
}

void Quad::SetDim(const Vector2<float>& ndim)
{
	m_Rect.SetDim(ndim);
	m_UpdateVB();
}

void Quad::SetX(float nx)
{
	m_Rect.SetX(nx);
	m_UpdateVB();
}

void Quad::SetY(float ny)
{
	m_Rect.SetY(ny);
	m_UpdateVB();
}

void Quad::SetWidth(float nwidth)
{
	m_Rect.SetWidth(nwidth);
	m_UpdateVB();
}

void Quad::SetHeight(float nheight)
{
	m_Rect.SetHeight(nheight);
	m_UpdateVB();
}

void Quad::SetAngle(float nangle)
{
	m_Rect.SetAngle(nangle);
	m_UpdateVB();
}

void Quad::SetRect(const Rect& nrect)
{
	m_Rect = nrect;
	m_UpdateVB();
}

bool Quad::ContainsAnyOf(const std::vector<Vector2<float>>& vertices) const
{
	return m_Rect.ContainsAnyOf(vertices);
}

bool Quad::ContainsAnyOf(const Rect& rect) const
{
	return m_Rect.ContainsAnyOf(rect);
}

bool Quad::ContainsAnyOf(const Quad& quad) const
{
	return m_Rect.ContainsAnyOf(quad.GetVertices());
}

bool Quad::Contains(const Vector2<float>& point) const
{
	return m_Rect.Contains(point);
}

bool Quad::IsIntersecting(const Rect& rect) const
{
	return m_Rect.IsIntersecting(rect);
}

bool Quad::IsIntersecting(const Quad& quad) const
{
	return m_Rect.IsIntersecting(quad.m_Rect);
}

void Quad::SetTextureCoords(const Texture& atlas, const Vector2<int>& index, const Vector2<int>& size)
{
	// Inverse width and height of atlas
	float inv_width = 1.0f / atlas.width;
	float inv_height = 1.0f / atlas.height;

	// Calculate faces
	float left = index.x * inv_width * size.x;
	float right = (index.x + 1) * inv_width * size.x;
	float bottom = index.y * inv_height * size.y;
	float top = (index.y + 1) * inv_height * size.y;

	// Create tex coords list
	std::vector<float> tex_coords = {
			left, bottom,
			right, bottom,
			right, top,
			left, top
	};

	// Update VertexBuffer coords
	vb->SetTex(tex_coords);
}

const VertexBuffer& Quad::GetVertexBuffer() const
{
	return *vb;
}

const IndexBuffer& Quad::GetIndexBuffer() const
{
	return *ib;
}

Quad::Quad()
	: m_Rect()
{
	m_ConstructBuffers();
}

Quad::Quad(float x, float y, float width, float height, float angle)
	: m_Rect(x, y, width, height, angle)
{
	m_ConstructBuffers();
}

Quad::Quad(const Vector2<float>& center, const Vector2<float>& dim, float angle)
	: m_Rect(center, dim, angle)
{
	m_ConstructBuffers();
}

Quad::Quad(const Rect& rect)
	: m_Rect(rect)
{
	m_ConstructBuffers();
}

Quad::Quad(const Quad& other)
	: m_Rect(other.m_Rect), vb(other.vb), ib(other.ib)
{
	// m_UpdateVB();
}

Vector2<float> Quad::GetCenter() const
{
	return m_Rect.GetCenter();
}

Vector2<float> Quad::GetDim() const
{
	return m_Rect.GetDim();
}

float Quad::GetX() const
{
	return m_Rect.GetX();
}

float Quad::GetY() const
{
	return m_Rect.GetY();
}

float Quad::GetWidth() const
{
	return m_Rect.GetWidth();
}

float Quad::GetHeight() const
{
	return m_Rect.GetHeight();
}

float Quad::GetAngle() const
{
	return m_Rect.GetAngle();
}

std::vector<Vector2<float>> Quad::GetVertices() const
{
	return m_Rect.GetVertices();
}

Vector2<float> Quad::BottomLeft() const
{
	return m_Rect.BottomLeft();
}

Vector2<float> Quad::BottomRight() const
{
	return m_Rect.BottomRight();
}

Vector2<float> Quad::TopRight() const
{
	return m_Rect.TopRight();
}

Vector2<float> Quad::TopLeft() const
{
	return m_Rect.TopLeft();
}

const Rect& Quad::GetRect() const
{
	return m_Rect;
}

std::string to_string(const Quad& quad)
{
	return to_string(quad.m_Rect);
}

bool operator==(const Quad& a, const Quad& b)
{
	return a.m_Rect == b.m_Rect;
}
