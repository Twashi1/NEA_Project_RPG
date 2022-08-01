#pragma once

#include "Core.h"
#include "Vector2.h"
#include "Math.h"
#include "Serialiser.h"

namespace Vivium {
	struct VIVIUM_API Rect : public IStreamable {
	protected:
		static const Vector2<float> m_VertexOffsets[4]; // For calculating vertices

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

		// NOTE: ignores rotation
		float Left() const;
		float Right() const;
		float Bottom() const;
		float Top() const;

		bool ContainsAnyOf(const std::vector<Vector2<float>>& vertices) const;
		bool ContainsAnyOf(const Rect& rect) const;
		bool Contains(const Vector2<float>& point) const;

		bool IsIntersecting(const Rect& rect) const;

		Rect();
		Rect(float x, float y, float width, float height, float angle = 0.0f);
		Rect(const Vector2<float>& center, const Vector2<float>& dim, float angle = 0.0f);
		Rect(const Rect& other);
		~Rect() = default;

		void operator=(const Rect& other) {
			x = other.x;
			y = other.y;
			width = other.width;
			height = other.height;
			angle = other.angle;
		}

		void Write(Serialiser& s) const override;
		void Read(Serialiser& s) override;
	};
}

bool operator==(const Vivium::Rect& a, const Vivium::Rect& b);