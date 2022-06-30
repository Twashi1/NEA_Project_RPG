#pragma once

#include <utility>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <functional>

template <typename T = int>
struct ENGINE_API Vector3 {
	static Vector3 ZERO;

	T x, y, z;

	Vector3() : x(0), y(0), z(0) {}
	Vector3(T x, T y, T z) : x(x), y(y), z(z) {}
	Vector3(const Vector3& copy) : x(copy.x), y(copy.y), z(copy.z) {}
	Vector3(Vector3&& move) : x(std::move(move.x)), y(std::move(move.y)), z(std::move(move.z)) {}

	// Negative operator (*-1)
	Vector3 operator-(void) const { return Vector3(-x, -y, -z); }

	// Basic arithmetic
	Vector3 operator+(const Vector3& other) const { return Vector3(x + other.x, y + other.y, z + other.z); }
	Vector3 operator-(const Vector3& other) const { return Vector3(x - other.x, y - other.y, z - other.z); }
	Vector3 operator*(const T& other) const { return Vector3(x * other, y * other, z * other); }
	Vector3 operator/(const T& other) const { return Vector3(x / other, y / other, z / other); }

	void operator+=(const Vector3& other) { x += other.x; y += other.y; z += other.z; }
	void operator-=(const Vector3& other) { x -= other.x; y -= other.y; z -= other.z; }
	void operator*=(const T& other) { x *= other; y *= other; z *= other; }
	void operator/=(const T& other) { x /= other; y /= other; z /= other; }

	// Miscellaneous
	Vector3 operator%(const T& other) const { return Vector3(x % other, y % other, z % other); }
	void operator%=(const T& other) { x %= other; y %= other; z %= other; }

	Vector3& operator=(const Vector3& other) {
		x = other.x;
		y = other.y;
		z = other.z;

		return *this;
	}

	// Logical
	bool operator<(const Vector3& other) const {
		// If x1 = x2 AND y1 = y2
		// Return z1 < z2
		// If x1 = x2 AND y1 != y2
		// Return  y1 < y2
		// If x1 != x2
		// Return x1 < x2
		if (x == other.x) {
			if (y == other.y) {
				return (z < other.z);
			}
			else {
				return (y < other.y);
			}
		}
		else {
			return x < other.x;
		}
	}

	bool operator>(const Vector3& other) const {
		// If x1 = x2 AND y1 = y2
		// Return z1 > z2
		// If x1 = x2 AND y1 != y2
		// Return  y1 > y2
		// If x1 != x2
		// Return x1 > x2
		if (x == other.x) {
			if (y == other.y) {
				return (z > other.z);
			}
			else {
				return (y > other.y);
			}
		}
		else {
			return x > other.x;
		}
	}

	// Distance from this vector to another
	T distance(const Vector3& other) const { return sqrt(pow(other.x - x, 2) + pow(other.y - y, 2) + pow(other.z - z, 2)); }
	// Dot product between this vector and another
	T dot(const Vector3& other) const { return (x * other.x) + (y * other.y) + (z * other.z); }
	// Magnitude of this vector
	T magnitude() const { return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)); }

	// Normalise this vector
	void normalise() { T inverse_magnitude = 1.0f / magnitude(); x *= inverse_magnitude; y *= inverse_magnitude; z *= inverse_magnitude; }

	operator Vector3<int>() { return Vector3<int>((int)x, (int)y, (int)z); }
	operator Vector3<long>() { return Vector3<long>((long)x, (long)y, (long)z); }
	operator Vector3<float>() { return Vector3<float>((float)x, (float)y, (float)z); }
	operator Vector3<double>() { return Vector3<double>((double)x, (double)y, (double)z); }
	operator Vector3<unsigned int>() { return Vector3<unsigned int>((unsigned int)x, (unsigned int)y, (unsigned int)z); }
	operator Vector3<uint8_t>() { return Vector3<uint8_t>((uint8_t)x, (uint8_t)y, (uint8_t)z); }
};

template <typename T>
std::string to_string(const Vector3<T>& v) {
	return std::format("[{}, {}, {}]", to_string(Utils::Round(v.x, 3)), to_string(Utils::Round(v.y, 3)), to_string(Utils::Round(v.z, 3)));
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector3<T>& vec) {
	os << "[" << vec.x << ", " << vec.y << ", " << vec.z << "]";

	return os;
}

template <typename T>
bool operator==(const Vector3<T>& a, const Vector3<T>& b) {
	return a.x == b.x && a.y == b.y && a.z == b.z;
}

template <typename T, typename ret_t = T>
Vector3<T> abs(const Vector3<T>& vec) { return Vector3<ret_t>(abs(vec.x), abs(vec.y), abs(vec.z)); }

template <typename T>
Vector3<T> Vector3<T>::ZERO = Vector2<T>(T(), T(), T());