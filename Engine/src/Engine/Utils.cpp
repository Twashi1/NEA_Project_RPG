#include "Utils.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Serialiser.h"

Vector3<float> ENGINE_API COLORS::BLACK    = { 0.00, 0.00, 0.00 };
Vector3<float> ENGINE_API COLORS::WHITE    = { 1.00, 1.00, 1.00 };
Vector3<float> ENGINE_API COLORS::RED      = { 1.00, 0.00, 0.00 };
Vector3<float> ENGINE_API COLORS::GREEN    = { 0.00, 1.00, 0.00 };
Vector3<float> ENGINE_API COLORS::BLUE     = { 0.00, 0.00, 1.00 };
Vector3<float> ENGINE_API COLORS::YELLOW   = { 1.00, 1.00, 0.00 };
Vector3<float> ENGINE_API COLORS::ORANGE   = { 1.00, 0.65, 0.00 };
Vector3<float> ENGINE_API COLORS::PURPLE   = { 1.00, 0.00, 1.00 };
Vector3<float> ENGINE_API COLORS::GRAY     = { 0.20, 0.20, 0.20 };
Vector3<float> ENGINE_API COLORS::DARKGRAY = { 0.12, 0.12, 0.12 };

ENGINE_API std::ostream& operator<<(std::ostream& os, const LOG& error)
{
	switch (error) {
	case LOG::INFO:
		os << "INFO"; break;
	case LOG::WARNING:
		os << "WARNING"; break;
	case LOG::FATAL:
		os << "FATAL"; break;
	default:
		ENG_LogWarn("Invalid LOG type {}", (uint8_t)error); os << "INVALID"; break;
	}

	return os;
}

ENGINE_API const std::chrono::system_clock::time_point Utils::Timer::compile_time = std::chrono::system_clock::now();

ENGINE_API bool Utils::CheckFileExists(const std::string& path)
{
	// Faster than std::filesystem::exists
	struct stat buffer;
	return !stat(path.c_str(), &buffer);
}

ENGINE_API bool Utils::CheckDirectoryExists(const std::string& path)
{
	return std::filesystem::exists(path);
}

ENGINE_API void Utils::CreateDirectory(const std::string& path)
{
	std::filesystem::create_directory(path);
}

ENGINE_API Vector2<float> Utils::RotatePoint(Vector2<float> point, Vector2<float> pivot, float angle) {
	Vector2<float> centered = point - pivot; // Subtract pivot so we rotate about origin

	// Compute trig values
	float cos_angle = cos(angle);
	float sin_angle = sin(angle);

	// Apply rotation by multiplying by rotation matrix
	float rotated_x = (centered.x * cos_angle) - (centered.y * sin_angle);
	float rotated_y = (centered.x * sin_angle) + (centered.y * cos_angle);

	return Vector2<float>(rotated_x, rotated_y) + pivot; // Add pivot back and return
}

ENGINE_API Vector2<float> Utils::RotatePointPrecomp(Vector2<float> point, Vector2<float> pivot, float cos_angle, float sin_angle) {
	Vector2<float> centered = point - pivot; // Subtract pivot so we rotate about origin

	// Apply rotation by multiplying by rotation matrix
	float rotated_x = (centered.x * cos_angle) - (centered.y * sin_angle);
	float rotated_y = (centered.x * sin_angle) + (centered.y * cos_angle);

	return Vector2<float>(rotated_x, rotated_y) + pivot; // Add pivot back and return
}

ENGINE_API uint32_t Utils::Factorial(uint32_t n)
{
	uint32_t val = 1;

	for (int i = 1; i <= n; ++i) {
		val *= i;
	}

	return val;
}

ENGINE_API float Utils::TriangleArea(Vector2<float> a, Vector2<float> b, Vector2<float> c)
{
	return std::abs(
		a.x * (b.y - c.y) +
		b.x * (c.y - a.y) + 
		c.x * (a.y - b.y)
	) * 0.5f;
}

ENGINE_API Vector2<float> Utils::ClampMax(Vector2<float> val, float max)
{
	return Vector2<float>(std::min(val.x, max), std::min(val.y, max));
}

ENGINE_API Vector2<float> Utils::ClampMin(Vector2<float> val, float min)
{
	return Vector2<float>(std::max(val.x, min), std::max(val.y, min));
}

ENGINE_API Vector2<float> Utils::Clamp(Vector2<float> val, float min, float max)
{
	Vector2<float> clamped = val;
	clamped.x = std::min(std::max(val.x, min), max);
	clamped.y = std::min(std::max(val.y, min), max);

	return clamped;
}

ENGINE_API void Utils::EraseSubstring(std::string& str, const std::string& substr)
{
	// Check substring exists in string
	if (str.find(substr) != std::string::npos) {
		str.erase(str.find(substr), substr.length());
	}
}

ENGINE_API void Utils::EraseSubstring(std::string& str, const char* substr)
{
	// Check substring exists in string
	if (str.find(substr) != std::string::npos) {
		str.erase(str.find(substr), strlen(substr));
	}
}

ENGINE_API bool Utils::EqualBias(float a, float b, float bias)
{
	return std::abs(a - b) <= bias;
}

ENGINE_API std::string Utils::EraseSubstring(const char* str, const std::string& substr)
{
	std::string result = str;
	// Check substring exists in string
	if (result.find(substr) != std::string::npos) {
		result.erase(result.find(substr), substr.length());
	}
	return result;
}

ENGINE_API std::string Utils::EraseSubstring(const char* str, const char* substr)
{
	std::string result = str;
	// Check substring exists in string
	if (result.find(substr) != std::string::npos) {
		result.erase(result.find(substr), strlen(substr));
	}
	return result;
}

ENGINE_API double Utils::Round(double val, int decimal_places)
{
	double ten_to_decimal_places = pow(10, decimal_places);
	double tmp = val * ten_to_decimal_places;

	if (tmp < 0) return ceil(tmp - 0.5) / ten_to_decimal_places;
	else return floor(tmp + 0.5) / ten_to_decimal_places;
}

ENGINE_API Vector2<float> Utils::Round(Vector2<float> val, int decimal_places)
{
	float ten_to_decimal_places = pow(10, decimal_places);
	float inverse_ten_to_decimal_places = 1.0 / ten_to_decimal_places;
	Vector2<float> tmp = val * ten_to_decimal_places;

	if (tmp.x < 0) tmp.x = ceil(tmp.x - 0.5) * inverse_ten_to_decimal_places;
	else tmp.x = floor(tmp.x + 0.5) * inverse_ten_to_decimal_places;

	if (tmp.y < 0) tmp.y = ceil(tmp.y - 0.5) * inverse_ten_to_decimal_places;
	else tmp.y = floor(tmp.y + 0.5) * inverse_ten_to_decimal_places;

	return tmp;
}

ENGINE_API Vector2<double> Utils::Round(Vector2<double> val, int decimal_places)
{
	double ten_to_decimal_places = pow(10, decimal_places);
	double inverse_ten_to_decimal_places = 1.0 / ten_to_decimal_places;
	Vector2<double> tmp = val * ten_to_decimal_places;

	if (tmp.x < 0) tmp.x = ceil(tmp.x - 0.5) * inverse_ten_to_decimal_places;
	else tmp.x = floor(tmp.x + 0.5) * inverse_ten_to_decimal_places;

	if (tmp.y < 0) tmp.y = ceil(tmp.y - 0.5) * inverse_ten_to_decimal_places;
	else tmp.y = floor(tmp.y + 0.5) * inverse_ten_to_decimal_places;

	return tmp;
}

ENGINE_API Vector2<float> Utils::Lerp(Vector2<float> start, Vector2<float> dest, float speed)
{
	return start + ((dest - start) * speed);
}

ENGINE_API std::string* Utils::ReadFile(const std::string& path)
{
	std::ifstream file(path, std::ios::in); // Create file pointer
	std::string* out = new std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()); // Read all text and store on heap
	
	if (*out == "") {
		ENG_LogWarn("File was empty/couldn't find file at {}", path);
	}

	return out; // Return pointer to text
}

ENGINE_API std::vector<std::string> Utils::SplitString(const std::string& s, const std::string& delim)
{
	// Ripped from https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
	size_t pos_start = 0, pos_end, delim_len = delim.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delim, pos_start)) != std::string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}

ENGINE_API void m_Log(const std::string& message, LOG error_type, const char* function, int line)
{
	std::string function_cleaned = function;
	// TODO: generalise removing __xyz (probably use regex)
	Utils::EraseSubstring(function_cleaned, "__cdecl ");
	Utils::EraseSubstring(function_cleaned, "__thiscall ");
	
	std::cout << "[" << Utils::Timer::GetTimeString() << "] " << "(" << error_type << ") " << function_cleaned << ":" << line << " " << message << std::endl;
	// Exit program if it was a fatal error
	if (error_type == LOG::FATAL) exit(EXIT_FAILURE);
}

ENGINE_API void m_Assert(const std::string& message, const char* function, int line)
{
	std::string function_cleaned = function;
	// TODO: generalise removing __xyz (probably use regex)
	Utils::EraseSubstring(function_cleaned, "__cdecl ");
	Utils::EraseSubstring(function_cleaned, "__thiscall ");

	std::cout << "[" << Utils::Timer::GetTimeString() << "]" << "Assertion failed (" << line << "): " << message << std::endl;

	exit(EXIT_FAILURE);
}

const long double Utils::Timer::m_NsToS = pow(10, -9);

ENGINE_API double Utils::Timer::GetTime()
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - compile_time).count() * m_NsToS;
}

ENGINE_API std::string Utils::Timer::GetTimeString()
{
	return std::format("{:%H:%M:%OS}", std::chrono::system_clock::now());
}

Utils::Timer::Timer()
	: m_Time(GetTime())
{}

Utils::Timer::Timer(double start_time)
	: m_Time(start_time)
{}

double Utils::Timer::GetElapsed()
{
	// Get current time
	double now = Utils::Timer::GetTime();
	// Calculate difference
	double elapsed = now - m_Time;
	// Update our time
	m_Time = now;
	// Return elapsed time
	return elapsed;
}
