#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <array>

#include "VersionNumber.h"
#include "Quad.h"
#include "Rect.h"

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

/* Serialise functions */

// Serialises an instance of object T
template <typename T>
extern ENGINE_API void Serialise(Serialiser& s, const T& data);

// Template specialisation for std::string
template <>
void ENGINE_API Serialise<std::string>(Serialiser& s, const std::string& data);

// Template specialisation for VersionNumber
template <>
void ENGINE_API Serialise<VersionNumber>(Serialiser& s, const VersionNumber& data);

// Template specialisation for Rect
template <>
void ENGINE_API Serialise<Rect>(Serialiser& s, const Rect& data);

// Template specialisation for Quad
template <>
void ENGINE_API Serialise<Quad>(Serialiser& s, const Quad& data);

// Serialises a vector
template <typename T>
extern ENGINE_API void Serialise(Serialiser& s, const std::vector<T>& data);

// Serialise an array, takes array and length of array
template <typename T>
extern ENGINE_API void Serialise(Serialiser& s, T* data, const uint32_t& length);

/* Deserialise functions */

template <typename T>
extern ENGINE_API T Deserialise(const Serialiser& s);

// Deserialise one instance of object type T and store in "memory" (assuming memory has already been allocated)
template <typename T>
extern ENGINE_API void Deserialise(const Serialiser& s, T* memory);

// Template specialisation for VersionNumber
template <>
void ENGINE_API Deserialise<VersionNumber>(const Serialiser& s, VersionNumber* memory);

// Template specialisation for std::string
template <>
void ENGINE_API Deserialise<std::string>(const Serialiser& s, std::string* memory);

// Template specialisation for Rect
template <>
void ENGINE_API Deserialise<Rect>(const Serialiser& s, Rect* memory);

// Template specialisation for Quad
template <>
void ENGINE_API Deserialise<Quad>(const Serialiser& s, Quad* memory);

// Deserialises a vector and stores in "memory"
template <typename T>
extern ENGINE_API void Deserialise(const Serialiser& s, std::vector<T>* memory);

// Deserialises an array of type T, storing it in buffer (which it returns) and storing amount of objects deserialised in length
// NOTE: buffer is created on heap, delete once done with the data
template <typename T>
extern ENGINE_API T* DeserialiseArray(const Serialiser& s, uint32_t* length);

template <typename T>
extern ENGINE_API void DeserialiseArray(const Serialiser& s, T* memory);
