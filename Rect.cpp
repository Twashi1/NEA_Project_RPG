#include "Rect.h"

Vector2<float> Rect::m_Offsets[4] = {
	{-1.0f, -1.0f }, // Bottom left corner
	{ 1.0f, -1.0f }, // Bottom right corner
	{ 1.0f,  1.0f }, // Top right corner
	{-1.0f,  1.0f }  // Top left corner
};

void Rect::m_UpdateTrig()
{
	cos_angle = std::cos(angle);
	sin_angle = std::sin(angle);
}

Vector2<float> Rect::GetCenter() const
{
	return center;
}

Vector2<float> Rect::GetDim() const
{
	return dim;
}

float Rect::GetX() const
{
	return x;
}

float Rect::GetY() const
{
	return y;
}

float Rect::GetWidth() const
{
	return width;
}

float Rect::GetHeight() const
{
	return height;
}

float Rect::GetAngle() const
{
	return angle;
}

std::vector<Vector2<float>> Rect::GetVertices() const
{
	std::vector<Vector2<float>> vertices(4);

	Vector2<float> halfdim = dim * 0.5f; // half width/height
	
	for (int i = 0; i < 4; i++) {
		vertices[i] = Utilities::RotatePointPrecomp(
			center + (halfdim * m_Offsets[i]), // Calculate vertex coordinates
			center, cos_angle, sin_angle
		);
	}

	return vertices;
}

Vector2<float> Rect::BottomLeft() const
{
	return Utilities::RotatePointPrecomp(center + (dim * 0.5f * m_Offsets[VERTEX::BOTTOMLEFT]), center, cos_angle, sin_angle);
}

Vector2<float> Rect::BottomRight() const
{
	return Utilities::RotatePointPrecomp(center + (dim * 0.5f * m_Offsets[VERTEX::BOTTOMRIGHT]), center, cos_angle, sin_angle);
}

Vector2<float> Rect::TopRight() const
{
	return Utilities::RotatePointPrecomp(center + (dim * 0.5f * m_Offsets[VERTEX::TOPRIGHT]), center, cos_angle, sin_angle);
}

Vector2<float> Rect::TopLeft() const
{
	return Utilities::RotatePointPrecomp(center + (dim * 0.5f * m_Offsets[VERTEX::TOPLEFT]), center, cos_angle, sin_angle);
}

bool Rect::Contains(const Vector2<float>& point) const
{
	const float epsilon = 0.1;

	std::vector<Vector2<float>> vertices = GetVertices();

	// Calculate triangle areas (where A, B, C, D = 0, 1, 2, 3)
	float apd = Utilities::TriangleArea(vertices[0], point, vertices[3]); // Triangle APD
	float dpc = Utilities::TriangleArea(vertices[3], point, vertices[2]); // Triangle DPC
	float cpb = Utilities::TriangleArea(vertices[2], point, vertices[1]); // Triangle CPB
	float pba = Utilities::TriangleArea(point, vertices[1], vertices[0]); // Triangle PBA

	// Sum triangle areas
	float total_area = apd + dpc + cpb + pba;

	// If sum of triangle areas is equal to the area of the rectangle, then the point is within the rectangle
	return Utilities::EqualBias(total_area, width * height, epsilon);
}

Rect::Rect(float x, float y, float width, float height, float angle)
	: x(x), y(y), width(width), height(height), angle(angle)
{
	m_UpdateTrig();
}

Rect::Rect(const Rect& other)
	: x(other.x), y(other.y), width(other.width), height(other.height), angle(other.angle)
{
	m_UpdateTrig();
}

std::string Rect::ToString() const
{
	std::stringstream ss;
	ss << "[" << BottomLeft() << ", " << BottomRight() << ", " << TopRight() << ", " << TopLeft() << "]";
	return ss.str();
}

Rect::Rect(const Vector2<float>& center, const Vector2<float>& dim, float angle)
	: center(center), dim(dim), angle(angle)
{
	m_UpdateTrig();
}

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

void Rect::SetCenter(const Vector2<float>& ncenter)
{
	center = ncenter;
}

void Rect::SetDim(const Vector2<float>& ndim)
{
	dim = ndim;
}

void Rect::SetX(float nx)
{
	x = nx;
}

void Rect::SetY(float ny)
{
	y = ny;
}

void Rect::SetWidth(float nwidth)
{
	width = nwidth;
}

void Rect::SetHeight(float nheight)
{
	height = nheight;
}

void Rect::SetAngle(float nangle)
{
	angle = nangle;
	m_UpdateTrig();
}

bool operator==(const Rect& a, const Rect& b)
{
	return a.x == b.x && a.y == b.y
		&& a.width == b.width && a.height == b.height
		&& a.angle == b.angle;
}

void Rect::SetCenter(float nx, float ny)
{
	x = nx;
	y = ny;
}

void Rect::SetDim(float nwidth, float nheight)
{
	width = nwidth;
	height = nheight;
}
