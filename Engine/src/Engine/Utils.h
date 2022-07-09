#pragma once
#include <cmath>
#include <string>
#include <filesystem>
#include <string>
#include <fstream>
#include <iostream>
#include <format>
#include <chrono>

#ifdef ENGINE_BUILD_DLL
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#define Log(msg, error_type) m_Log(msg, error_type, __FUNCSIG__, __LINE__);

using std::to_string;

template <typename T>
struct Vector2;

template <typename T>
struct Vector3;

class ENGINE_API COLORS {
public:
	static Vector3<float> BLACK;
	static Vector3<float> WHITE;
	static Vector3<float> RED;
	static Vector3<float> GREEN;
	static Vector3<float> BLUE;
	static Vector3<float> YELLOW;
	static Vector3<float> PURPLE;
	static Vector3<float> ORANGE;
	static Vector3<float> GRAY;
	static Vector3<float> DARKGRAY;
};

enum class ENGINE_API LOG : uint8_t {
	INFO,
	WARNING,
	FATAL // Fatal errors will terminate the program when logged
}; 

ENGINE_API std::ostream& operator<<(std::ostream& os, const LOG& error);

namespace Utils {
	class ENGINE_API Timer {
	public:
		static const std::chrono::system_clock::time_point compile_time;

		static double GetTime();
		static std::string GetTimeString();
	};

	// A wrapper for std::vector which implements simplified remove and erase methods
	template <typename T>
	struct ENGINE_API List {
	private:
		using ListData_t = std::vector<T>;

		ListData_t m_data;

	public:
		// Adds on object to the list
		void Push(const T& object) { m_data.push_back(object); }
		// Removes all occurences of the object from the list
		void Remove(const T& object)
		{
			// Remove all occurences of shader from m_Shaders, and return new end of list
			auto new_end = std::remove(m_data.begin(), m_data.end(), object);

			// Erase all elements from the new end of the list to the old end of the list
			m_data.erase(new_end, m_data.end());
		}
		// Removes the index from the list
		void Erase(unsigned int index)
		{
			if (index < m_data.size()) {
				m_data.erase(m_data.begin() + index);
			}
			else {
				std::string text = std::format("Index out of bounds: {} > {}", to_string(index), to_string(m_data.size()));
				Log(text, LOG::FATAL);
			}
		}
		// Reserves size to fit n objects
		void Reserve(unsigned int n) { m_data.reserve(n * sizeof(T)); }
		// Returns size of object
		unsigned int Size() { return m_data.size(); }

		/* Iterator functions */
		ListData_t::iterator begin() { return m_data.begin(); }
		ListData_t::iterator end() { return m_data.end(); }
	};

	ENGINE_API std::string GetTimeString();

	ENGINE_API bool CheckFileExists(const std::string& path);
	ENGINE_API bool CheckDirectoryExists(const std::string& path);

	ENGINE_API void CreateDirectory(const std::string& path);

	// Rotate a point around another pivot point counterclockwise by "angle"
	ENGINE_API Vector2<float> RotatePoint(Vector2<float> point, Vector2<float> pivot, float angle);
	// Rotate a point around another pivot point counterclockwise, taking the precomputed values of cos(angle) and sin(angle)
	ENGINE_API Vector2<float> RotatePointPrecomp(Vector2<float> point, Vector2<float> pivot, float cos_angle, float sin_angle);

	ENGINE_API uint32_t Factorial(uint32_t n);

	// Calculates area of triangle abc
	ENGINE_API float TriangleArea(Vector2<float> a, Vector2<float> b, Vector2<float> c);

	ENGINE_API Vector2<float> ClampMax(Vector2<float> val, float max);
	ENGINE_API Vector2<float> ClampMin(Vector2<float> val, float min);

	ENGINE_API Vector2<float> Clamp(Vector2<float> val, float min, float max);

	// Removes substr from str
	ENGINE_API void EraseSubstring(std::string& str, const std::string& substr);
	// Removes substr from str
	ENGINE_API void EraseSubstring(std::string& str, const char* substr);

	// Returns true if difference between a and b is less than the bias
	ENGINE_API bool EqualBias(float a, float b, float bias);

	// Returns substr removed from str
	ENGINE_API std::string EraseSubstring(const char* str, const std::string& substr);
	// Returns substr removed from str
	ENGINE_API std::string EraseSubstring(const char* str, const char* substr);

	// Rounds val to decimal_places
	ENGINE_API double Round(double val, int decimal_places);
	ENGINE_API Vector2<float> Round(Vector2<float> val, int decimal_places);
	ENGINE_API Vector2<double> Round(Vector2<double> val, int decimal_places);

	// Linearly interpolate between two points at speed
	ENGINE_API Vector2<float> Lerp(Vector2<float> start, Vector2<float> dest, float speed);

	// Reads an entire file into one string on heap (NOTE: remember to delete the string once done with it)
	ENGINE_API std::string* ReadFile(const std::string& path);

	// Splits string by delimiter
	ENGINE_API std::vector<std::string> SplitString(const std::string& s, const std::string& delim);
}

ENGINE_API void m_Log(const std::string& message, LOG error_type, const char* function, int line);