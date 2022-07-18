#pragma once

#include "Utils.h"

namespace Serialiser {
	class Streamable;

	template <typename T>
	concept __Streamable = std::is_base_of_v<Streamable, T>
		|| std::is_same_v<T, int8_t>
		|| std::is_same_v<T, uint8_t>
		|| std::is_same_v<T, int16_t>
		|| std::is_same_v<T, uint16_t>
		|| std::is_same_v<T, int32_t>
		|| std::is_same_v<T, uint32_t>
		|| std::is_same_v<T, long>
		|| std::is_same_v<T, float>
		|| std::is_same_v<T, double>
		|| std::is_same_v<T, std::string>;

	class ENGINE_API Stream {
	public:
		std::ofstream* out;
		std::ifstream* in;

		// Open the file we're deserialising from
		void BeginRead(const char* path);
		// Close the file we're serialising to
		void BeginWrite(const char* path);
		// Close the file we're deserialising from
		void EndRead();
		// Close the file we're serialising to
		void EndWrite();

		Stream();
		~Stream();
	};

	class ENGINE_API Streamable {
	public:
		Streamable();
		~Streamable();

		void Write(Stream& s);
		void Read(Stream& s);
	};

	// Serialises one instance of object
	template <__Streamable T> ENGINE_API extern void Serialise(Stream& s, const T& data);
	template <> ENGINE_API void Serialise<std::string>(Stream& s, const std::string& data);

	// Unserialises and returns one instance of type
	template <__Streamable T> ENGINE_API extern T Unserialise(Stream& s);
	template <> ENGINE_API std::string Unserialise<std::string>(Stream& s);

	// Unserialise one instance of object type T and store in "memory" (assuming memory has already been allocated)
	template <__Streamable T> ENGINE_API extern void Unserialise(Stream& s, T* memory);
	template <> ENGINE_API void Unserialise<std::string>(Stream& s, std::string* memory);
}
