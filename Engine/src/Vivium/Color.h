#pragma once

#include "Core.h"
#include "Logger.h"

namespace Vivium {
	struct HSVColor;

	struct VIVIUM_API RGBColor {
	private:
		char m_DigitToHex(const uint8_t& digit) const;

	public:
		float r, g, b;

		RGBColor(float r, float g, float b);
		RGBColor(uint8_t r, uint8_t g, uint8_t b);
		RGBColor(const std::string& hex_string);

		std::string GetHex() const;
	};
}