#pragma once

#include "Core.h"
#include "Logger.h"
#include "Vector2.h"

namespace Vivium {
	namespace Math {
		const float PI = 3.14159265358979323f;

		VIVIUM_API Vector2<float> Rotate(const Vector2<float>& point, float angle, const Vector2<float>& pivot = NULL);
		VIVIUM_API Vector2<float> Rotate(const Vector2<float>& point, float cos_angle, float sin_angle, const Vector2<float>& pivot = NULL);

		VIVIUM_API unsigned int Factorial(unsigned int n);

		VIVIUM_API float TriangleArea(const Vector2<float>& a, const Vector2<float>& b, const Vector2<float>& c);

		VIVIUM_API bool EqualBias(float a, float b, float bias);
		VIVIUM_API double Round(double val, int decimal_places);
		VIVIUM_API Vector2<float> Round(Vector2<float> val, int decimal_places);

		VIVIUM_API float DegreesToRadians(float degrees);
		VIVIUM_API float RadiansToDegrees(float radians);

		struct VIVIUM_API AABB {
			float x, y, hw, hh; // center and half dimensions

			AABB(float x, float y, float w, float h);
			AABB(const AABB& other) = default;

			bool IsIntersecting(const AABB& other);
		};
	}
}