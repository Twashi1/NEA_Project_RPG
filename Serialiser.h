#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <array>

#include "Tile.h"
#include "VersionNumber.h"

class Serialiser {
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
void Serialise(Serialiser& s, const T& data)
{
	s.outstream->write((char*)&data, sizeof(T));
}

// Template specialisation for std::string
template <>
void Serialise<std::string>(Serialiser& s, const std::string& data);

// Template specialisation for VersionNumber
template <>
void Serialise<VersionNumber>(Serialiser& s, const VersionNumber& v);

// Serialises a vector
template <typename T>
void Serialise(Serialiser& s, const std::vector<T>& data)
{
	uint32_t length = data.size();
	Serialise<T>(s, length); // Serialise length of list

	// Serialise elements in list
	for (const T& element : data) {
		Serialise<T>(s, element);
	}
}

// Serialise an array, takes array and length of array
template <typename T>
void Serialise(Serialiser& s, T* data, const uint32_t& length)
{
	Serialise<T>(s, length); // Serialise length of list

	// Serialise each element in array
	for (uint32_t i = 0; i < length; i++) {
		Serialise<T>(s, data[i]);
	}
}

/* Deserialise functions */

template <typename T>
T Deserialise(const Serialiser& s)
{
	// T must have default constructor
	assert(std::is_default_constructible<T>::value);

	char* readbuff = new char[sizeof(T)]; // Allocate buffer for file data
	s.instream->read(readbuff, sizeof(T)); // Read file data into buffer

	T instance;
	memcpy(&instance, readbuff, sizeof(T)); // Copy data from buffer into pointer given

	delete[] readbuff; // Delete buffer after copying the data

	return instance;
}

// Deserialise one instance of object type T and store in "memory" (assuming memory has already been allocated)
template <typename T>
void Deserialise(const Serialiser& s, T* memory)
{
	char* readbuff = new char[sizeof(T)]; // Allocate buffer for file data
	s.instream->read(readbuff, sizeof(T)); // Read file data into buffer

	memcpy(memory, readbuff, sizeof(T)); // Copy data from buffer into pointer given

	delete[] readbuff; // Delete buffer after copying the data
}

// Deserialises a version number
template <>
void Deserialise<VersionNumber>(const Serialiser& s, VersionNumber* memory);

// Deserialises a string and stores in "memory"
template <>
void Deserialise<std::string>(const Serialiser& s, std::string* memory);

// Deserialises a vector and stores in "memory"
template <typename T>
void Deserialise(const Serialiser& s, std::vector<T>* memory)
{
	uint32_t length = Deserialise<uint32_t>(s); // Get length of vector
	memory->reserve(length * sizeof(T)); // Reserve space needed for vector

	for (uint32_t i = 0; i < length; i++) {
		memory->push_back(Deserialise<T>(s)); // Deserialise and push to vector
	}
}

// Deserialises an array of type T, storing it in buffer (which it returns) and storing amount of objects deserialised in length
// NOTE: buffer is created on heap, delete once done with the data
template <typename T>
T* DeserialiseArray(const Serialiser& s, uint32_t* length)
{
	Deserialise<uint32_t>(s, length); // Deserialise length of array

	T* buffer = new T[*length]; // Create buffer of length on heap

	for (uint32_t i = 0; i < *length; i++) {
		buffer[i] = Deserialise<T>(s); // Deserialise and add to buffer
	}

	return buffer;
}
