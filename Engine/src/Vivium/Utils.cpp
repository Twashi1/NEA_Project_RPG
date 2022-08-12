#include "Utils.h"

namespace Vivium {
	void Utils::EraseSubstring(std::string& str, const std::string& substr)
	{
		// Check substring exists in string
		if (str.find(substr) != std::string::npos) {
			str.erase(str.find(substr), substr.length());
		}
	}

	void Utils::EraseSubstring(std::string& str, const char* substr)
	{
		// Check substring exists in string
		if (str.find(substr) != std::string::npos) {
			str.erase(str.find(substr), strlen(substr));
		}
	}

	std::string Utils::EraseSubstring(const char* str, const std::string& substr)
	{
		std::string result = str;
		// Check substring exists in string
		if (result.find(substr) != std::string::npos) {
			result.erase(result.find(substr), substr.length());
		}
		return result;
	}

	std::string Utils::EraseSubstring(const char* str, const char* substr)
	{
		std::string result = str;
		// Check substring exists in string
		if (result.find(substr) != std::string::npos) {
			result.erase(result.find(substr), strlen(substr));
		}
		return result;
	}

	std::string* Utils::ReadFile(const std::string& path)
	{
		if (!std::filesystem::exists(path)) { LogError("Couldn't find file at {}", path); }

		std::ifstream file(path, std::ios::in); // Create file pointer
		std::string* out = new std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()); // Read all text and store on heap

		if (*out == "") {
			LogWarn("File was empty/couldn't find file at {}", path);
		}

		return out; // Return pointer to text
	}

	std::vector<std::string> Utils::SplitString(const std::string& s, const std::string& delim)
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

	VIVIUM_API void Utils::SaveAsBitmap(unsigned char* data, int width, int height, int bpp, const char* path)
	{
		std::ofstream file(path, std::ios::binary | std::ios::trunc);
		if 
			(!file) return;

		// save bitmap file headers
		BITMAPFILEHEADER fileHeader;
		BITMAPINFOHEADER* infoHeader;
		infoHeader = (BITMAPINFOHEADER*)malloc(sizeof(BITMAPINFOHEADER));
		RGBQUAD bl = { 0,0,0,0 };  //black color
		RGBQUAD wh = { 0xff,0xff,0xff,0xff }; // white color


		fileHeader.bfType = 0x4d42;
		fileHeader.bfSize = 0;
		fileHeader.bfReserved1 = 0;
		fileHeader.bfReserved2 = 0;
		fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + (sizeof(BITMAPINFOHEADER)) + 2 * sizeof(RGBQUAD);

		infoHeader->biSize = (sizeof(BITMAPINFOHEADER));
		infoHeader->biWidth = width;
		infoHeader->biHeight = height;
		infoHeader->biPlanes = 1;
		infoHeader->biBitCount = 1;
		infoHeader->biCompression = BI_RGB; //no compression needed
		infoHeader->biSizeImage = 0;
		infoHeader->biXPelsPerMeter = 0;
		infoHeader->biYPelsPerMeter = 0;
		infoHeader->biClrUsed = 2;
		infoHeader->biClrImportant = 2;

		file.write((char*)&fileHeader, sizeof(fileHeader)); //write bitmapfileheader
		file.write((char*)infoHeader, (sizeof(BITMAPINFOHEADER))); //write bitmapinfoheader
		file.write((char*)&bl, sizeof(bl)); //write RGBQUAD for black
		file.write((char*)&wh, sizeof(wh)); //write RGBQUAD for white

		// convert the bits into bytes and write the file
		int offset, numBytes = ((width + 31) / 32) * 4;
		byte* bytes = (byte*)malloc(numBytes * sizeof(byte));

		for (int y = height - 1; y >= 0; --y) {
			offset = y * width;
			memset(bytes, 0, (numBytes * sizeof(byte)));
			for (int x = 0; x < width; ++x)
				if (data[offset++]) {
					bytes[x / 8] |= 1 << (7 - x % 8);
				};
			file.write((const char*)bytes, numBytes);
		};
		free(bytes);
		file.close();
	}
}