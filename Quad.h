#pragma once

#include "Utilities.h"
#include "Vector2.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Rect.h"

struct Quad {
private:
	Rect m_Rect;

	// Buffers
	std::shared_ptr<VertexBuffer> vb;
	std::shared_ptr<IndexBuffer> ib;

	// Update vertex buffer
	void m_UpdateVB();

	// Construct buffers
	void m_ConstructBuffers();

public:
	const VertexBuffer& GetVertexBuffer() const;
	const IndexBuffer& GetIndexBuffer()   const;

	Quad(float x, float y, float width, float height, float angle);
	Quad(const Vector2<float>& center, const Vector2<float>& dim, float angle);
	Quad(const Rect& rect);
	Quad(const Quad& other);

	Vector2<float> GetCenter() const;
	Vector2<float> GetDim() const;

	float GetX() const;
	float GetY() const;
	float GetWidth() const;
	float GetHeight() const;
	float GetAngle() const;

	std::vector<Vector2<float>> GetVertices() const;
	Vector2<float> BottomLeft() const;
	Vector2<float> BottomRight() const;
	Vector2<float> TopRight() const;
	Vector2<float> TopLeft() const;

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

	bool ContainsAnyOf(const std::vector<Vector2<float>>& vertices) const;
	bool ContainsAnyOf(const Rect& rect) const;
	bool ContainsAnyOf(const Quad& quad) const;
	bool Contains(const Vector2<float>& point) const;

	bool IsIntersecting(const Rect& rect) const;
	bool IsIntersecting(const Quad& quad) const;

	friend bool operator==(const Quad& a, const Quad& b);

	std::string ToString() const;
};

bool operator==(const Quad& a, const Quad& b);