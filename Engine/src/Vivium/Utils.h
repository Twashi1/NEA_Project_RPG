#pragma once

#include "Core.h"
#include "Logger.h"

namespace Vivium {
	namespace Utils {
		template <typename T>
		void Remove(std::vector<T>& data, const T& object)
		{
			if (data.size() == 0) { LogWarn("Erasing object from empty list"); return; }
			// Remove all occurences of object from data, and return new end of list
			auto new_end = std::remove(data.begin(), data.end(), object);

			// Erase all elements from the new end of the list to the old end of the list
			data.erase(new_end, data.end());
		}

		// Removes substr from str
		VIVIUM_API void EraseSubstring(std::string& str, const ::std::string& substr);
		// Removes substr from str
		VIVIUM_API void EraseSubstring(std::string& str, const char* substr);

		// Returns substr removed from str
		VIVIUM_API std::string EraseSubstring(const char* str, const ::std::string& substr);
		// Returns substr removed from str
		VIVIUM_API std::string EraseSubstring(const char* str, const char* substr);

		// Reads an entire file into one string on heap (NOTE: remember to delete the string once done with it)
		VIVIUM_API std::string* ReadFile(const ::std::string& path);

		// Splits string by delimiter
		VIVIUM_API std::vector<std::string> SplitString(const ::std::string& s, const ::std::string& delim);
	}
}