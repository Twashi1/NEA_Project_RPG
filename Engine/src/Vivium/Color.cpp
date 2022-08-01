#include "Color.h"

namespace Vivium {
	char RGBColor::m_DigitToHex(const uint8_t& digit) const {
		// ok sure its a little ugly but i cba
		switch ((int)digit) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			return static_cast<char>(digit + '0');
		case 10: return 'A';
		case 11: return 'B';
		case 12: return 'C';
		case 13: return 'D';
		case 14: return 'E';
		case 15: return 'F';
		default:
			LogError("Invalid digit value of {}", (int)digit); return '?';
		}
	}
	RGBColor::RGBColor(float r, float g, float b)
		: r(r), g(g), b(b)
	{}

	RGBColor::RGBColor(uint8_t r, uint8_t g, uint8_t b)
		: r(float(r) / 255.0f), g(float(g) / 255.0f), b(float(b) / 255.0f)
	{}

	RGBColor::RGBColor(const std::string& hex_string)
	{
		// TODO: Use some regex to clean up a # or 0x or whatever
		// TODO: this is just kinda dumb, like if im gonna use stoi
		//	might as well split the string up into 3 parts and stoi each part

		uint32_t hex_value = stoi(hex_string, 0, 16);
		uint8_t ri = hex_value >> 16;
		uint8_t gi = (hex_value >> 8) - (ri << 8);
		uint8_t bi = hex_value - (gi << 8) - (ri << 16);

		r = ri / 255.0f;
		g = gi / 255.0f;
		b = bi / 255.0f;
	}
	 
	std::string RGBColor::GetHex() const
	{
		std::array<float, 3> values = { r, g, b };

		std::string hex_string;

		for (float v : values) {
			uint8_t byte_value = v * 255;
			uint8_t second_digit = v / 16;
			uint8_t first_digit = byte_value - second_digit * 16;

			hex_string += m_DigitToHex(second_digit);
			hex_string += m_DigitToHex(first_digit);
		}

		return hex_string;
	}
}
