#include "Utilities.h"

Vector3<float> COLORS::BLACK  = { 0.00, 0.00, 0.00 };
Vector3<float> COLORS::WHITE  = { 1.00, 1.00, 1.00 };
Vector3<float> COLORS::RED    = { 1.00, 0.00, 0.00 };
Vector3<float> COLORS::GREEN  = { 0.00, 1.00, 0.00 };
Vector3<float> COLORS::BLUE   = { 0.00, 0.00, 1.00 };
Vector3<float> COLORS::YELLOW = { 1.00, 1.00, 0.00 };
Vector3<float> COLORS::ORANGE = { 1.00, 0.65, 0.00 };
Vector3<float> COLORS::PURPLE = { 1.00, 0.00, 1.00 };

std::ostream& Utilities::operator<<(std::ostream& os, const Utilities::ERROR& error)
{
	switch (error) {
	case Utilities::ERROR::INFO:
		os << "INFO"; break;
	case Utilities::ERROR::WARNING:
		os << "WARNING"; break;
	case Utilities::ERROR::FATAL:
		os << "FATAL"; break;
	}

	return os;
}

bool Utilities::CheckFileExists(const std::string& path)
{
	// Faster than std::filesystem::exists
	struct stat buffer;
	return !stat(path.c_str(), &buffer);
}

bool Utilities::CheckDirectoryExists(const std::string& path)
{
	return std::filesystem::exists(path);
}

void Utilities::CreateDirectory(const std::string& path)
{
	std::filesystem::create_directory(path);
}

Vector2<float> Utilities::RotatePoint(Vector2<float> point, Vector2<float> pivot, float angle) {
	Vector2<float> centered = point - pivot; // Subtract pivot so we rotate about origin

	// Precompute trig values
	float cos_angle = cos(angle);
	float sin_angle = sin(angle);

	// Apply rotation by multiplying by rotation matrix
	float rotated_x = (centered.x * cos_angle) - (centered.y * sin_angle);
	float rotated_y = (centered.x * sin_angle) + (centered.y * cos_angle);

	return Vector2<float>(rotated_x, rotated_y) + pivot; // Add pivot back and return
}

Vector2<float> Utilities::RotatePointPrecomp(Vector2<float> point, Vector2<float> pivot, float cos_angle, float sin_angle) {
	Vector2<float> centered = point - pivot; // Subtract pivot so we rotate about origin

	// Apply rotation by multiplying by rotation matrix
	float rotated_x = (centered.x * cos_angle) - (centered.y * sin_angle);
	float rotated_y = (centered.x * sin_angle) + (centered.y * cos_angle);

	return Vector2<float>(rotated_x, rotated_y) + pivot; // Add pivot back and return
}

uint32_t Utilities::Factorial(uint32_t n)
{
	uint32_t val = 1;

	for (int i = 1; i <= n; ++i) {
		val *= i;
	}

	return val;
}

float Utilities::TriangleArea(Vector2<float> a, Vector2<float> b, Vector2<float> c)
{
	return std::abs(
		a.x * (b.y - c.y) +
		b.x * (c.y - a.y) + 
		c.x * (a.y - b.y)
	) * 0.5f;
}

float Utilities::ClampMax(float val, float max)
{
	return std::min(val, max);
}

float Utilities::ClampMin(float val, float min)
{
	return std::max(val, min);
}

Vector2<float> Utilities::ClampMax(Vector2<float> val, float max)
{
	return Vector2<float>(std::min(val.x, max), std::min(val.y, max));
}

Vector2<float> Utilities::ClampMin(Vector2<float> val, float min)
{
	return Vector2<float>(std::max(val.x, min), std::max(val.y, min));
}

Vector2<float> Utilities::Clamp(Vector2<float> val, float min, float max)
{
	Vector2<float> clamped = val;
	clamped.x = std::min(std::max(val.x, min), max);
	clamped.y = std::min(std::max(val.y, min), max);

	return clamped;
}

void Utilities::EraseSubstring(std::string& str, const std::string& substr)
{
	// Check substring exists in string
	if (str.find(substr) != std::string::npos) {
		str.erase(str.find(substr), substr.length());
	}
}

void Utilities::EraseSubstring(std::string& str, const char* substr)
{
	// Check substring exists in string
	if (str.find(substr) != std::string::npos) {
		str.erase(str.find(substr), strlen(substr));
	}
}

bool Utilities::EqualBias(float a, float b, float bias)
{
	return std::abs(a - b) <= bias;
}

std::string Utilities::EraseSubstring(const char* str, const std::string& substr)
{
	std::string result = str;
	// Check substring exists in string
	if (result.find(substr) != std::string::npos) {
		result.erase(result.find(substr), substr.length());
	}
	return result;
}

std::string Utilities::EraseSubstring(const char* str, const char* substr)
{
	std::string result = str;
	// Check substring exists in string
	if (result.find(substr) != std::string::npos) {
		result.erase(result.find(substr), strlen(substr));
	}
	return result;
}

double Utilities::Round(double val, int decimal_places)
{
	double ten_to_decimal_places = pow(10, decimal_places);
	double tmp = val * ten_to_decimal_places;

	if (tmp < 0) return ceil(tmp - 0.5) / ten_to_decimal_places;
	else return floor(tmp + 0.5) / ten_to_decimal_places;
}

Vector2<float> Utilities::Round(Vector2<float> val, int decimal_places)
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

Vector2<double> Utilities::Round(Vector2<double> val, int decimal_places)
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

Vector2<float> Utilities::Lerp(Vector2<float> start, Vector2<float> dest, float speed)
{
	return start + ((dest - start) * speed);
}

void m_Log(const std::string& message, Utilities::ERROR error_type, const char* function, int line)
{
	std::string function_cleaned = function;
	// TODO: generalise removing __xyz
	Utilities::EraseSubstring(function_cleaned, "__cdecl ");
	Utilities::EraseSubstring(function_cleaned, "__thiscall ");

	std::cout << "[" << error_type << "] " << function_cleaned << ":" << line << " " << message << std::endl;
	// Exit program if it was a fatal error
	if (error_type == Utilities::ERROR::FATAL) exit(EXIT_FAILURE);
}
