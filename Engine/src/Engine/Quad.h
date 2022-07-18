#pragma once

#include "Utils.h"
#include "Vector2.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Rect.h"
#include "Texture.h"

struct ENGINE_API Quad : public Serialiser::Streamable, protected Rect {
private:
	std::vector<float> m_TexCoords;

	static IndexBuffer* ib;

	// Buffers
	ENG_Ptr(VertexBuffer) vb;

	// Update vertex buffer
	void m_UpdateVB();

	// Construct buffers
	void m_ConstructBuffers();

public:
	static void Init();
	static void Terminate();

	// Make methods public
	using Rect::Contains;
	using Rect::ContainsAnyOf;
	using Rect::BottomLeft;
	using Rect::BottomRight;
	using Rect::TopRight;
	using Rect::TopLeft;
	using Rect::GetVertices;

	const VertexBuffer& GetVertexBuffer() const;
	static const IndexBuffer& GetIndexBuffer();

	Quad();
	Quad(float x, float y, float width, float height, float angle);
	Quad(const Vector2<float>& center, const Vector2<float>& dim, float angle);
	Quad(float x, float y, float width, float height, float angle, const std::vector<float>& tex_coords);
	Quad(const Vector2<float>& center, const Vector2<float>& dim, float angle, const std::vector<float>& tex_coords);
	Quad(const Rect& rect);
	Quad(const Quad& other);
	~Quad();

	Vector2<float> GetCenter() const;
	Vector2<float> GetDim() const;

	float GetX() const;
	float GetY() const;
	float GetWidth() const;
	float GetHeight() const;
	float GetAngle() const;

	const Rect& GetRect() const;

	void SetCenter(const Vector2<float>& ncenter);
	void SetCenter(float nx, float ny);
	void SetDim(const Vector2<float>& ndim);
	void SetDim(float nwidth, float nheight);

	void SetX(float nx);
	void SetY(float ny);
	void SetWidth(float nwidth);
	void SetHeight(float nheight);
	void SetAngle(float nangle);

	void SetRect(const Rect& nrect);

	const std::vector<float>& GetTexCoords() const;

	bool ContainsAnyOf(const Quad& quad) const;
	bool IsIntersecting(const Quad& quad) const;

	// Changes texture coordinates so a specific texture from a texture atlas can be loaded
	// Takes the atlas being loaded, the index of the image within the atlas, and the size (in px) of each image in the atlas
	void SetTextureCoords(const Texture& atlas, const Vector2<int>& index, const Vector2<int>& size);
	void SetTextureCoords(const Texture& atlas, const Vector2<int>& top_left, const Vector2<int>& bottom_right, const Vector2<int>& size);

	friend bool operator==(const Quad& a, const Quad& b);
	friend std::string to_string(const Quad& quad);

	void Write(Serialiser::Stream& s);
	void Read(Serialiser::Stream& s);
};

std::string to_string(const Quad& quad);

bool operator==(const Quad& a, const Quad& b);