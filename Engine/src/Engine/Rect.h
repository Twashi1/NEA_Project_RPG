#pragma once

#include "Vector2.h"
#include "Utils.h"

struct Quad;

struct ENGINE_API Rect {
private:
	static Vector2<float> m_Offsets[4];

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
	float cos_angle = 0.0f, sin_angle = 1.0f; // Store precomputed values for cos/sin of angle

	void m_UpdateTrig(); // Updates precomputed values for cos/sin of angle

public:
	// For indexing the return value of GetVertices
	enum VERTEX : int {
		BOTTOMLEFT = 0,
		BOTTOMRIGHT = 1,
		TOPRIGHT = 2,
		TOPLEFT = 3
	};

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

	void SetCenter(const Vector2<float>& ncenter);
	void SetCenter(float nx, float ny);
	void SetDim(const Vector2<float>& ndim);
	void SetDim(float nwidth, float nheight);
	
	void SetX(float nx);
	void SetY(float ny);
	void SetWidth(float nwidth);
	void SetHeight(float nheight);
	void SetAngle(float nangle);

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
		cos_angle = other.cos_angle;
		sin_angle = other.sin_angle;
	}

	friend bool operator==(const Rect& a, const Rect& b);
	friend std::string to_string(const Rect& rect);
	friend Quad;
};

bool operator==(const Rect& a, const Rect& b);
std::string to_string(const Rect& rect);