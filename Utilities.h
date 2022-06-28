#pragma once
#include <cmath>
#include <string>
#include <filesystem>
#include <string>
#include <fstream>
#include <iostream>

#include "Vector2.h"
#include "Vector3.h"

#define Log(msg, error_type) m_Log(msg, error_type, __FUNCSIG__, __LINE__);

class COLORS {
public:
	static Vector3<float> BLACK;
	static Vector3<float> WHITE;
	static Vector3<float> RED;
	static Vector3<float> GREEN;
	static Vector3<float> BLUE;
	static Vector3<float> YELLOW;
	static Vector3<float> PURPLE;
	static Vector3<float> ORANGE;
};

namespace Utilities {
	enum class ERROR : uint8_t {
		INFO,
		WARNING,
		FATAL // Fatal errors will terminate the program when logged
	};

	std::ostream& operator<<(std::ostream& os, const Utilities::ERROR& error);

	bool CheckFileExists(const std::string& path);
	bool CheckDirectoryExists(const std::string& path);

	void CreateDirectory(const std::string& path);

	// Rotate a point around another pivot point counterclockwise by "angle"
	Vector2<float> RotatePoint(Vector2<float> point, Vector2<float> pivot, float angle);
	// Rotate a point around another pivot point counterclockwise, taking the precomputed values of cos(angle) and sin(angle)
	Vector2<float> RotatePointPrecomp(Vector2<float> point, Vector2<float> pivot, float cos_angle, float sin_angle);

	uint32_t Factorial(uint32_t n);

	// Calculates area of triangle abc
	float TriangleArea(Vector2<float> a, Vector2<float> b, Vector2<float> c);

	// Exclusively clamps val to max
	float ClampMax(float val, float max);
	// Exclusively clamps val to min
	float ClampMin(float val, float min);

	Vector2<float> ClampMax(Vector2<float> val, float max);
	Vector2<float> ClampMin(Vector2<float> val, float min);

	Vector2<float> Clamp(Vector2<float> val, float min, float max);

	// Removes substr from str
	void EraseSubstring(std::string& str, const std::string& substr);
	// Removes substr from str
	void EraseSubstring(std::string& str, const char* substr);

	// Returns true if difference between a and b is less than the bias
	bool EqualBias(float a, float b, float bias);

	// Returns substr removed from str
	std::string EraseSubstring(const char* str, const std::string& substr);
	// Returns substr removed from str
	std::string EraseSubstring(const char* str, const char* substr);

	// Rounds val to decimal_places
	double Round(double val, int decimal_places);
	Vector2<float> Round(Vector2<float> val, int decimal_places);
	Vector2<double> Round(Vector2<double> val, int decimal_places);

	// Linearly interpolate between two points at speed
	Vector2<float> Lerp(Vector2<float> start, Vector2<float> dest, float speed);

	// Reads an entire file into one string on heap (NOTE: remember to delete the string once done with it)
	std::string* ReadFile(const std::string& path);

	// Splits string by delimiter
	std::vector<std::string> SplitString(const std::string& s, const std::string& delim);
}

void m_Log(const std::string& message, Utilities::ERROR error_type, const char* function, int line);