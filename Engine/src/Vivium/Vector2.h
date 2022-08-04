#pragma once

#include "Core.h"
#include "Logger.h"

namespace Vivium {
	template <typename T> concept Arithmetic = std::is_arithmetic_v<T>;
	template <typename T> concept Signed = std::is_signed_v<T>;
	template <typename T> concept Integral = std::is_integral_v<T>;
	template <typename T> concept FloatingPoint = std::is_floating_point_v<T>;

	template <typename T> requires Arithmetic<T>
	struct VIVIUM_API Vector2 {
	public:
		using value_type = T;

		T x, y;

		Vector2() : x(T()), y(T()) {};

		Vector2(T v) : x(v), y(v) {};
		Vector2(T x, T y) : x(x), y(y) {};

		Vector2(const Vector2& other) = default;
		Vector2(Vector2&& other) noexcept = default;

		Vector2& operator=(const Vector2& other) = default;

		Vector2 operator+(const Vector2& other) const { return Vector2(x + other.x, y + other.y); }
		Vector2 operator-(const Vector2& other) const { return Vector2(x - other.x, y - other.y); }
		Vector2 operator*(const Vector2& other) const { return Vector2(x * other.x, y * other.y); }
		Vector2 operator/(const Vector2& other) const { return Vector2(x / other.x, y / other.y); }

		Vector2 operator*(const T& other) const { return Vector2(x * other, y * other); }
		Vector2 operator/(const T& other) const { return Vector2(x / other, y / other); }

		void operator+=(const Vector2& other) { x += other.x; y += other.y; }
		void operator-=(const Vector2& other) { x -= other.x; y -= other.y; }
		void operator*=(const Vector2& other) { x *= other.x; y *= other.y; }
		void operator/=(const Vector2& other) { x /= other.x; y /= other.y; }

		void operator*=(const T& other) { x *= other; y *= other; }
		void operator/=(const T& other) { x /= other; y /= other; }

		Vector2 operator-() const requires Signed<T> { return Vector2(-x, -y); }

		Vector2 operator%(const T& other) const requires Integral<T> { return Vector2(x % other, y % other); }
		void operator%=(const T& other) requires Integral<T> { x %= other; y %= other; }

		bool operator<(const Vector2& other) { return x == other.x ? y < other.y : x < other.x; }
		bool operator>(const Vector2& other) { return x == other.x ? y > other.y : x > other.x; }
		friend bool operator==(const Vector2<T>& a, const Vector2<T>& b) { return a.x == b.x && a.y == b.y; }
		friend bool operator!=(const Vector2<T>& a, const Vector2<T>& b) { return a.x != b.x || a.y != b.y; }

		explicit operator glm::vec2() const { return glm::vec2(x, y); }

		Vector2 floor() const requires FloatingPoint<T> { return Vector2(std::floor(x), std::floor(y)); }
		Vector2 ceil() const requires FloatingPoint<T> { return Vector2(std::floor(x), std::floor(y)); }

		// Cast to arithmetic type
		template <typename C>
		explicit operator Vector2<C>() const requires Arithmetic<C> { return Vector2<C>((C)x, (C)y); }
	};
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vivium::Vector2<T>& v) {
	os << "[" << v.x << ", " << v.y << "]";

	return os;
}

namespace std {
	template <typename T>
	struct hash<Vivium::Vector2<T>> {
		unsigned int operator()(const Vivium::Vector2<T>& k) const noexcept {
			unsigned int x_hash = std::hash<T>()(k.x);
			unsigned int y_hash = std::hash<T>()(k.y) << 1;
			return x_hash ^ y_hash;
		}
	};

	template <typename T>
	struct formatter<Vivium::Vector2<T>> : formatter<string> {
		auto format(Vivium::Vector2<T> v, format_context& ctx) {
			return formatter<string>::format(
				std::format("[{}, {}]", v.x, v.y), ctx);
		}
	};
}