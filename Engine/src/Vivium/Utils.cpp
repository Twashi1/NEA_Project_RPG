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
}