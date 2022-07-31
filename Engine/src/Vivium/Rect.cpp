#include "Rect.h"

namespace Vivium {
	const Vector2<float> Rect::m_VertexOffsets[4] = {
		{-1.0f, -1.0f }, // Bottom left corner
		{ 1.0f, -1.0f }, // Bottom right corner
		{ 1.0f,  1.0f }, // Top right corner
		{-1.0f,  1.0f }  // Top left corner
	};

	std::vector<Vector2<float>> Rect::GetVertices() const
	{
		std::vector<Vector2<float>> vertices(4);

		Vector2<float> halfdim = dim * 0.5f; // Calculate half width and height

		float cos_angle = cos(angle);
		float sin_angle = sin(angle);

		for (int i = 0; i < 4; i++) {
			vertices[i] = Math::Rotate(
				center + (halfdim * m_VertexOffsets[i]),
				cos_angle, sin_angle,
				center
			);
		}

		return vertices;
	}

	Vector2<float> Rect::BottomLeft() const
	{
		return Math::Rotate(center + (dim * 0.5f * m_VertexOffsets[VERTEX::BOTTOMLEFT]), angle, center);
	}

	Vector2<float> Rect::BottomRight() const
	{
		return Math::Rotate(center + (dim * 0.5f * m_VertexOffsets[VERTEX::BOTTOMRIGHT]), angle, center);
	}

	Vector2<float> Rect::TopRight() const
	{
		return Math::Rotate(center + (dim * 0.5f * m_VertexOffsets[VERTEX::TOPRIGHT]), angle, center);
	}

	Vector2<float> Rect::TopLeft() const
	{
		return Math::Rotate(center + (dim * 0.5f * m_VertexOffsets[VERTEX::TOPLEFT]), angle, center);
	}

	float Rect::Left() const
	{
		return x - width * 0.5f;
	}

	float Rect::Right() const
	{
		return x + width * 0.5f;
	}

	float Rect::Bottom() const
	{
		return y - height * 0.5f;
	}

	float Rect::Top() const
	{
		return y + height * 0.5f;
	}

	bool Rect::Contains(const Vector2<float>& point) const
	{
		const float epsilon = 0.1;

		std::vector<Vector2<float>> vertices = GetVertices();

		// Calculate triangle areas (where A, B, C, D = 0, 1, 2, 3)
		float apd = Math::TriangleArea(vertices[0], point, vertices[3]); // Triangle APD
		float dpc = Math::TriangleArea(vertices[3], point, vertices[2]); // Triangle DPC
		float cpb = Math::TriangleArea(vertices[2], point, vertices[1]); // Triangle CPB
		float pba = Math::TriangleArea(point, vertices[1], vertices[0]); // Triangle PBA

		// Sum triangle areas
		float total_area = apd + dpc + cpb + pba;

		// If sum of triangle areas is equal to the area of the rectangle, then the point is within the rectangle
		return Math::EqualBias(total_area, width * height, epsilon);
	}

	Rect::Rect()
		: x(0.0f), y(0.0f), width(0.0f), height(0.0f), angle(0.0f)
	{
	}

	Rect::Rect(float x, float y, float width, float height, float angle)
		: x(x), y(y), width(width), height(height), angle(angle)
	{}

	Rect::Rect(const Rect& other)
		: x(other.x), y(other.y), width(other.width), height(other.height), angle(other.angle)
	{}

	Rect::Rect(const Vector2<float>& center, const Vector2<float>& dim, float angle)
		: center(center), dim(dim), angle(angle)
	{}

	bool Rect::IsIntersecting(const Rect& rect) const
	{
		return ContainsAnyOf(rect.GetVertices()) || rect.ContainsAnyOf(GetVertices());
	}

	bool Rect::ContainsAnyOf(const Rect& rect) const
	{
		std::vector<Vector2<float>> vertices = rect.GetVertices();

		for (const Vector2<float>& v : vertices) {
			// If we contain the vertex
			if (Contains(v)) return true;
		}

		return false;
	}

	bool Rect::ContainsAnyOf(const std::vector<Vector2<float>>& vertices) const
	{
		for (const Vector2<float>& v : vertices) {
			// If we contain the vertex
			if (Contains(v)) return true;
		}

		return false;
	}

	bool operator==(const Rect& a, const Rect& b)
	{
		return a.x == b.x && a.y == b.y
			&& a.width == b.width && a.height == b.height
			&& a.angle == b.angle;
	}
}

bool operator==(const Vivium::Rect& a, const Vivium::Rect& b)
{
	return a.x == b.x && a.y == b.y
		&& a.width == b.width && a.height == b.height
		&& a.angle == b.angle;
}