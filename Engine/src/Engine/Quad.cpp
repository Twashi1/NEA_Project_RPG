#include "Quad.h"

namespace CoolEngineName {
	IndexBuffer* Quad::ib = nullptr;

	void Quad::m_UpdateVB()
	{
		std::vector<Vector2<float>> vertices = GetVertices();

		// Set vertex buffer vertices
		vb->Set(
			{
				vertices[Rect::BOTTOMLEFT].x, vertices[Rect::BOTTOMLEFT].y,
				vertices[Rect::BOTTOMRIGHT].x, vertices[Rect::BOTTOMRIGHT].y,
				vertices[Rect::TOPRIGHT].x, vertices[Rect::TOPRIGHT].y,
				vertices[Rect::TOPLEFT].x, vertices[Rect::TOPLEFT].y
			}
		);
	}

	void Quad::m_ConstructBuffers()
	{
		std::vector<Vector2<float>> vertices = GetVertices();

		m_TexCoords = {
				0.0f, 0.0f,
				1.0f, 0.0f,
				1.0f, 1.0f,
				0.0f, 1.0f
		};

		// Generate vertex and texture coordinates
		vb = ENG_MakePtr(VertexBuffer,
			std::vector<float>{
			vertices[Rect::BOTTOMLEFT].x, vertices[Rect::BOTTOMLEFT].y,
				vertices[Rect::BOTTOMRIGHT].x, vertices[Rect::BOTTOMRIGHT].y,
				vertices[Rect::TOPRIGHT].x, vertices[Rect::TOPRIGHT].y,
				vertices[Rect::TOPLEFT].x, vertices[Rect::TOPLEFT].y
		},
			m_TexCoords
				);
	}

	void Quad::Init()
	{
		ib = new IndexBuffer({ 0, 1, 2, 2, 3, 0 });
	}

	void Quad::Terminate()
	{
		delete ib;
	}

	void Quad::SetCenter(float nx, float ny)
	{
		x = nx;
		y = ny;
		m_UpdateVB();
	}

	void Quad::SetCenter(const Vector2<float>& ncenter)
	{
		center = ncenter;
		m_UpdateVB();
	}

	void Quad::SetDim(float nwidth, float nheight)
	{
		width = nwidth;
		height = nheight;
		m_UpdateVB();
	}

	void Quad::SetDim(const Vector2<float>& ndim)
	{
		dim = ndim;
		m_UpdateVB();
	}

	void Quad::SetX(float nx)
	{
		x = nx;
		m_UpdateVB();
	}

	void Quad::SetY(float ny)
	{
		y = ny;
		m_UpdateVB();
	}

	void Quad::SetWidth(float nwidth)
	{
		width = nwidth;
		m_UpdateVB();
	}

	void Quad::SetHeight(float nheight)
	{
		height = nheight;
		m_UpdateVB();
	}

	void Quad::SetAngle(float nangle)
	{
		angle = nangle;
		m_UpdateVB();
	}

	void Quad::SetRect(const Rect& nrect)
	{
		x = nrect.x; y = nrect.y;
		width = nrect.width; height = nrect.height;
		angle = nrect.angle;
		m_UpdateVB();
	}

	const std::vector<float>& Quad::GetTexCoords() const
	{
		return m_TexCoords;
	}

	bool Quad::ContainsAnyOf(const Quad& quad) const
	{
		return ContainsAnyOf(quad.GetVertices());
	}

	bool Quad::IsIntersecting(const Quad& quad) const
	{
		return ContainsAnyOf(quad.GetVertices()) || quad.ContainsAnyOf(GetVertices());
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
		m_TexCoords = {
				left, bottom,
				right, bottom,
				right, top,
				left, top
		};

		// Update VertexBuffer coords
		vb->SetTex(m_TexCoords);
	}

	void Quad::SetTextureCoords(const Texture& atlas, const Vector2<int>& top_left, const Vector2<int>& bottom_right, const Vector2<int>& size)
	{
		// Inverse width and height of atlas
		float inv_width = 1.0f / atlas.width;
		float inv_height = 1.0f / atlas.height;

		// Calculate faces
		float left = top_left.x * inv_width * size.x;
		float right = (bottom_right.x + 1) * inv_width * size.x;
		float bottom = bottom_right.y * inv_height * size.y;
		float top = (top_left.y + 1) * inv_height * size.y;

		// Create tex coords list
		m_TexCoords = {
				left, bottom,
				right, bottom,
				right, top,
				left, top
		};

		// Update VertexBuffer coords
		vb->SetTex(m_TexCoords);
	}

	void Quad::Write(Serialiser::Stream& s) const
	{
		Serialise<decltype(x)>(s, x);
		Serialise<decltype(y)>(s, y);
		Serialise<decltype(width)>(s, width);
		Serialise<decltype(height)>(s, height);
		Serialise<decltype(angle)>(s, angle);

		ENG_Assert(m_TexCoords.size() == 8, "Invalid amount of texCoords, was {}, expected {}", m_TexCoords.size(), 8);

		for (float coord : m_TexCoords) {
			Serialise<float>(s, coord);
		}
	}

	void Quad::Read(Serialiser::Stream& s)
	{
		Unserialise<decltype(x)>(s, &x);
		Unserialise<decltype(y)>(s, &y);
		Unserialise<decltype(width)>(s, &width);
		Unserialise<decltype(height)>(s, &height);
		Unserialise<decltype(angle)>(s, &angle);

		// Sets size of vector to 8, so vector correctly reads elements
		m_TexCoords.resize(8);

		for (int i = 0; i < 8; i++) {
			Unserialise<float>(s, &m_TexCoords[i]);
		}
	}

	const VertexBuffer& Quad::GetVertexBuffer() const
	{
		return *vb;
	}

	const IndexBuffer& Quad::GetIndexBuffer()
	{
		return *ib;
	}

	Quad::Quad()
		: m_TexCoords()
	{
		m_ConstructBuffers();
	}

	Quad::Quad(float x, float y, float width, float height, float angle)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		this->angle = angle;

		m_ConstructBuffers();
	}

	Quad::Quad(const Vector2<float>& center, const Vector2<float>& dim, float angle)
	{
		this->center = center;
		this->dim = dim;
		this->angle = angle;

		m_ConstructBuffers();
	}

	Quad::Quad(float x, float y, float width, float height, const std::vector<float>& tex_coords, float angle)
		: m_TexCoords(tex_coords)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		this->angle = angle;

		m_ConstructBuffers();
	}

	Quad::Quad(const Vector2<float>& center, const Vector2<float>& dim, const std::vector<float>& tex_coords, float angle)
		: m_TexCoords(tex_coords)
	{
		this->center = center;
		this->dim = dim;
		this->angle = angle;

		m_ConstructBuffers();
	}

	Quad::Quad(const Rect& rect)
	{
		x = rect.x; y = rect.y;
		width = rect.width; height = rect.height;
		angle = rect.angle;

		m_ConstructBuffers();
	}

	Quad::Quad(const Quad& other)
		: vb(other.vb), m_TexCoords(other.m_TexCoords)
	{
		x = other.x; y = other.y;
		width = other.width; height = other.height;
		angle = other.angle;
	}

	Quad::~Quad()
	{}

	Vector2<float> Quad::GetCenter() const
	{
		return center;
	}

	Vector2<float> Quad::GetDim() const
	{
		return dim;
	}

	float Quad::GetX() const
	{
		return x;
	}

	float Quad::GetY() const
	{
		return y;
	}

	float Quad::GetWidth() const
	{
		return width;
	}

	float Quad::GetHeight() const
	{
		return height;
	}

	float Quad::GetAngle() const
	{
		return angle;
	}

	const Rect& Quad::GetRect() const
	{
		return Rect(x, y, width, height, angle);
	}

	std::string to_string(const Quad& quad)
	{
		return std::format("[{}, {}, {}, {}]",
			to_string(quad.BottomLeft()),
			to_string(quad.BottomRight()),
			to_string(quad.TopRight()),
			to_string(quad.TopLeft())
		);
	}

	bool operator==(const Quad& a, const Quad& b)
	{
		return a.x == b.x && a.y == b.y &&
			a.width == b.width && a.height == b.height &&
			a.angle == b.angle;
	}
}