#pragma once

#include "Vector2.h"
#include "Utils.h"

struct ENGINE_API Rect {
protected:
	static Vector2<float> m_Offsets[4];

public:
	// For indexing the return value of GetVertices
	enum VERTEX : int {
		BOTTOMLEFT = 0,
		BOTTOMRIGHT = 1,
		TOPRIGHT = 2,
		TOPLEFT = 3
	};

	// Store as vector2, or x, y
	union {
		Vector2<float> center;
		struct { float x, y; };
	};

	// Store as vector2, or width, height
	union {
		Vector2<float> dim;
		struct { float width, height; };
	};

	float angle;

	std::vector<Vector2<float>> GetVertices() const;
	Vector2<float> BottomLeft() const;
	Vector2<float> BottomRight() const;
	Vector2<float> TopRight() const;
	Vector2<float> TopLeft() const;

	bool ContainsAnyOf(const std::vector<Vector2<float>>& vertices) const;
	bool ContainsAnyOf(const Rect& rect) const;
	bool Contains(const Vector2<float>& point) const;

	bool IsIntersecting(const Rect& rect) const;

	Rect();
	Rect(float x, float y, float width, float height, float angle);
	Rect(const Vector2<float>& center, const Vector2<float>& dim, float angle);
	Rect(const Rect& other);
	
	void operator=(const Rect& other) {
		x = other.x;
		y = other.y;
		width = other.width;
		height = other.height;
		angle = other.angle;
	}
};

bool operator==(const Rect& a, const Rect& b);
std::string to_string(const Rect& rect);