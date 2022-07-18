#include "Rect.h"

Vector2<float> Rect::m_Offsets[4] = {
	{-1.0f, -1.0f }, // Bottom left corner
	{ 1.0f, -1.0f }, // Bottom right corner
	{ 1.0f,  1.0f }, // Top right corner
	{-1.0f,  1.0f }  // Top left corner
};

std::vector<Vector2<float>> Rect::GetVertices() const
{
	this;
	std::vector<Vector2<float>> vertices(4);

	Vector2<float> halfdim = dim * 0.5f; // half width/height
	
	for (int i = 0; i < 4; i++) {
		vertices[i] = Utils::RotatePoint(
			center + (halfdim * m_Offsets[i]), // Calculate vertex coordinates
			center,
			angle
		);
	}

	return vertices;
}

Vector2<float> Rect::BottomLeft() const
{
	return Utils::RotatePoint(center + (dim * 0.5f * m_Offsets[VERTEX::BOTTOMLEFT]), center, angle);
}

Vector2<float> Rect::BottomRight() const
{
	return Utils::RotatePoint(center + (dim * 0.5f * m_Offsets[VERTEX::BOTTOMRIGHT]), center, angle);
}

Vector2<float> Rect::TopRight() const
{
	return Utils::RotatePoint(center + (dim * 0.5f * m_Offsets[VERTEX::TOPRIGHT]), center, angle);
}

Vector2<float> Rect::TopLeft() const
{
	return Utils::RotatePoint(center + (dim * 0.5f * m_Offsets[VERTEX::TOPLEFT]), center, angle);
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
	float apd = Utils::TriangleArea(vertices[0], point, vertices[3]); // Triangle APD
	float dpc = Utils::TriangleArea(vertices[3], point, vertices[2]); // Triangle DPC
	float cpb = Utils::TriangleArea(vertices[2], point, vertices[1]); // Triangle CPB
	float pba = Utils::TriangleArea(point, vertices[1], vertices[0]); // Triangle PBA

	// Sum triangle areas
	float total_area = apd + dpc + cpb + pba;

	// If sum of triangle areas is equal to the area of the rectangle, then the point is within the rectangle
	return Utils::EqualBias(total_area, width * height, epsilon);
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

Rect::~Rect()
{
}

void Rect::Write(Serialiser::Stream& s) const
{
	Serialise<decltype(x)>(s, x);
	Serialise<decltype(y)>(s, y);
	Serialise<decltype(width)>(s, width);
	Serialise<decltype(height)>(s, height);
	Serialise<decltype(angle)>(s, angle);
}

void Rect::Read(Serialiser::Stream& s)
{
	Unserialise<decltype(x)>(s, &x);
	Unserialise<decltype(y)>(s, &y);
	Unserialise<decltype(width)>(s, &width);
	Unserialise<decltype(height)>(s, &height);
	Unserialise<decltype(angle)>(s, &angle);
}


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

std::string to_string(const Rect& rect)
{
	return std::format("[{}, {}, {}, {}]",
		to_string(rect.BottomLeft()),
		to_string(rect.BottomRight()),
		to_string(rect.TopRight()),
		to_string(rect.TopLeft())
	);
}