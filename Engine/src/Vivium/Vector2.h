#pragma once

#include "Core.h"

namespace Vivium {
	template <typename T> requires Arithmetic<T>
	struct VIVIUM_API Vector2 {
	public:
		using value_type = T; // Unused

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

		friend bool operator<(const Vector2<T>& a, const Vector2<T>& b) { return a.x == b.x ? a.y < b.y : a.x < b.x; }
		friend bool operator>(const Vector2<T>& a, const Vector2<T>& b) { return a.x == b.x ? a.y > b.y : a.x > b.x; }
		friend bool operator<=(const Vector2<T>& a, const Vector2<T>& b) { return a.x == b.x ? a.y <= b.y : a.x <= b.x; }
		friend bool operator>=(const Vector2<T>& a, const Vector2<T>& b) { return a.x == b.x ? a.y >= b.y : a.x >= b.x; }
		friend bool operator==(const Vector2<T>& a, const Vector2<T>& b) { return a.x == b.x && a.y == b.y; }
		friend bool operator!=(const Vector2<T>& a, const Vector2<T>& b) { return a.x != b.x || a.y != b.y; }

		explicit operator glm::vec2() const { return glm::vec2(x, y); }

		Vector2 floor() const requires FloatingPoint<T> { return Vector2(std::floor(x), std::floor(y)); }
		Vector2 ceil() const requires FloatingPoint<T> { return Vector2(std::ceil(x), std::ceil(y)); }
		Vector2 normalise() const { return *this * InverseSquareRoot(x * x + y * y); }
		Vector2 fract() const requires FloatingPoint<T>
		{
			float int_part;
			float fx = std::modf(x, &int_part);
			float fy = std::modf(y, &int_part);
			
			return Vector2<float>(fx, fy);
		}

		Vector2 abs() const requires Signed<T> { return Vector2(std::abs(x), std::abs(y)); }

		T length() const { return std::sqrt(x * x + y * y); }

		T magnitude() const { return std::sqrt(x * x + y * y); }

		T distance(const Vector2& other) const
		{ 
			T dx = x - other.x;
			T dy = y - other.y;
			return std::sqrt(dx * dx + dy * dy);
		}

		T sqr_distance(const Vector2& other) const
		{
			T dx = x - other.x;
			T dy = y - other.y;
			return dx * dx + dy * dy;
		}

		T MaxComponent() const { return std::max(x, y); }

		// Cast to arithmetic type
		template <typename C>
		operator Vector2<C>() const requires Arithmetic<C> { return Vector2<C>((C)x, (C)y); }

		friend std::ostream& operator<<(std::ostream& os, const Vector2& v) {
			os << "[" << v.x << ", " << v.y << "]";

			return os;
		}
	};

	// Stolen from boost
	template <typename T>
	inline void hash_combine(std::size_t& seed, const T& v) {
		std::hash<T> hasher;
		seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}
}

namespace std {
	template <typename T>
	struct hash<Vivium::Vector2<T>> {
		std::size_t operator()(const Vivium::Vector2<T>& k) const noexcept {
			std::size_t hash = 0;
			Vivium::hash_combine<T>(hash, k.x);
			Vivium::hash_combine<T>(hash, k.y);

			return hash;
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