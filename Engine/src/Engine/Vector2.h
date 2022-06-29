#pragma once

#include <utility>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <functional>
#include <iomanip>

#include "Utilities.h"

template <typename T = int>
struct ENGINE_API Vector2 {
	static Vector2 ZERO;

	T x, y;

	Vector2() : x(0), y(0) {}
	Vector2(T x) : x(x), y(x) {}
	Vector2(T x, T y) : x(x), y(y) {}
	Vector2(const Vector2& copy) : x(copy.x), y(copy.y) {}
	Vector2(Vector2&& move) noexcept : x(std::move(move.x)), y(std::move(move.y)) {}

	// Negative operator (*-1)
	Vector2 operator-(void) const { return Vector2(-x, -y); }

	// Basic arithmetic
	Vector2 operator+(const Vector2& other) const { return Vector2(x + other.x, y + other.y); }
	Vector2 operator-(const Vector2& other) const { return Vector2(x - other.x, y - other.y); }
	Vector2 operator*(const T& other) const { return Vector2(x * other, y * other); }
	Vector2 operator/(const T& other) const { return Vector2(x / other, y / other); }

	Vector2 operator*(const Vector2& other) const { return Vector2(x * other.x, y * other.y); }
	Vector2 operator/(const Vector2& other) const { return Vector2(x / other.x, y / other.y); }

	void operator+=(const Vector2& other) { x += other.x; y += other.y; }
	void operator-=(const Vector2& other) { x -= other.x; y -= other.y; }
	void operator*=(const T& other) { x *= other; y *= other; }
	void operator/=(const T& other) { x /= other; y /= other; }
	void operator*=(const Vector2& other) { x *= other.x; y *= other.y; }
	void operator/=(const Vector2& other) { x /= other.x; y /= other.y; }

	// Miscellaneous
	Vector2 operator%(const T& other) const { return Vector2(x % other, y % other); }
	void operator%=(const T& other) { x %= other; y %= other; }

	Vector2& operator=(const Vector2& other) {
		x = other.x;
		y = other.y;

		return *this;
	}

	// Bitwise operations (vector)
	Vector2 operator^(const Vector2& other) const { return Vector2(x ^ other.x, y ^ other.y); }
	Vector2 operator&(const Vector2& other) const { return Vector2(x & other.x, y & other.y); }
	Vector2 operator|(const Vector2& other) const { return Vector2(x | other.x, y | other.y); }

	void operator^=(const Vector2& other) { x ^= other.x; y ^= other.y; }
	void operator&=(const Vector2& other) { x &= other.x; y &= other.y; }
	void operator|=(const Vector2& other) { x |= other.x; y |= other.y; }

	// Bitwise operations (scalar)
	Vector2 operator^(const unsigned int other) const { return Vector2(x ^ other, y ^ other); }
	Vector2 operator&(const unsigned int other) const { return Vector2(x & other, y & other); }
	Vector2 operator|(const unsigned int other) const { return Vector2(x | other, y | other); }

	void operator^=(const unsigned int other) { x ^= other; y ^= other; }
	void operator&=(const unsigned int other) { x &= other; y &= other; }
	void operator|=(const unsigned int other) { x |= other; y |= other; }

	// Shifts
	Vector2 operator<<(const unsigned int other) const { return Vector2(x << other, y << other); }
	Vector2 operator>>(const unsigned int other) const { return Vector2(x >> other, y >> other); }

	// Logical
	bool operator<(const Vector2& other) const {
		if (x == other.x) { return (y < other.y); }
		else { return x < other.x; }
	}

	bool operator>(const Vector2& other) const {
		if (x == other.x) { return (y > other.y); }
		else { return x > other.x; }
	}

	bool operator!=(const Vector2& other) const {
		return x != other.x || y != other.y;
	}

	// Distance from this vector to another
	T distance(const Vector2& other) const { return sqrt(pow(other.x - x, 2) + pow(other.y - y, 2)); }
	// Dot product between this vector and another
	T dot(const Vector2& other) const { return (x * other.x) + (y * other.y); }
	// Cross product between this vector and another
	T cross(const Vector2& other) const { return (x * other.y) - (y * other.x); }
	// Magnitude of this vector
	T magnitude() const { return sqrt(pow(x, 2) + pow(y, 2)); }

	// Normalise this vector
	void normalise() { T inverse_magnitude = 1.0f / magnitude(); x *= inverse_magnitude; y *= inverse_magnitude; }

	operator Vector2<int>()			 { return Vector2<int>((int)x, (int)y);							   }
	operator Vector2<long>()		 { return Vector2<long>((long)x, (long)y);						   }
	operator Vector2<float>()		 { return Vector2<float>((float)x, (float)y);					   }
	operator Vector2<double>()		 { return Vector2<double>((double)x, (double)y);				   }
	operator Vector2<unsigned int>() { return Vector2<unsigned int>((unsigned int)x, (unsigned int)y); }
	operator Vector2<std::uint8_t>() { return Vector2<std::uint8_t>((std::uint8_t)x, (std::uint8_t)y); }
};

template <typename T>
std::string to_string(const Vector2<T>& v) {
	return std::format("[{}, {}]", to_string(Utilities::Round(v.x, 3)), to_string(Utilities::Round(v.y, 3)));
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector2<T>& vec) {
	os << "[" << std::fixed << std::setprecision(3) << vec.x << ", " << std::fixed << std::setprecision(3) << vec.y << "]";

	return os;
}

template <typename T>
bool operator==(const Vector2<T>& a, const Vector2<T>& b) {
	return a.x == b.x && a.y == b.y;
}

template <typename T, typename ret_t = T>
Vector2<T> abs(const Vector2<T>& vec) { return Vector2<ret_t>(abs(vec.x), abs(vec.y)); }

namespace std {
	template <>
	struct hash<Vector2<int>> {
		unsigned int operator()(const Vector2<int>& k) const noexcept {
			unsigned int x_hash = std::hash<int>()(k.x);
			unsigned int y_hash = std::hash<int>()(k.y) << 1;
			return x_hash ^ y_hash;
		}
	};
}

template <typename T>
Vector2<T> Vector2<T>::ZERO = Vector2<T>(T(), T());

template <typename T>
Vector2<T> cross(Vector2<T> v, T a) {
	return Vector2<T>(a * v.y, -a * v.x);
}

template <typename T>
Vector2<T> cross(T a, Vector2<T> v) {
	return Vector2<T>(-a * v.y, a * v.x);
}

template <typename T>
T cross(Vector2<T> a, Vector2<T> b) {
	return (a.x * b.y) - (a.y * b.x);
}