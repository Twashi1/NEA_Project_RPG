#pragma once

#include "Utils.h"

class ENGINE_API Serialiser {
public:
	std::ofstream* outstream;
	std::ifstream* instream;

	// Open the file we're deserialising from
	void BeginRead(const char* path);
	// Close the file we're serialising to
	void BeginWrite(const char* path);
	// Close the file we're deserialising from
	void EndRead();
	// Close the file we're serialising to
	void EndWrite();

	Serialiser();
	~Serialiser();
};

class ENGINE_API Serialiseable {
public:
	void Load(Serialiser& s);
	void Unload(Serialiser& s) const;
};

// Serialises one instance of object
template <typename T> ENGINE_API extern void Serialise(Serialiser& s, const T& data);
template <> ENGINE_API void Serialise<std::string>(Serialiser& s, const std::string& data);

// Deserialises and returns one instance of type
template <typename T> ENGINE_API extern T Deserialise(Serialiser& s);
template <> ENGINE_API std::string Deserialise<std::string>(Serialiser& s);

// Deserialise one instance of object type T and store in "memory" (assuming memory has already been allocated)
template <typename T> ENGINE_API extern void Deserialise(Serialiser& s, T* memory);
template <> ENGINE_API void Deserialise<std::string>(Serialiser& s, std::string* memory);
