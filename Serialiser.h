#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <array>

#include "Tile.h"

/*
Serialisation examples
------

Serialiser s; // Create serialiser instance

------
Vectors
------
std::vector<int> original{0, 1, 2, 8, 7, 9, 15, 3, 2}; // Our test data

s.BeginWrite("data.txt"); // Open the file we are serialising to
s.Serialise(original); // Write in the vector
s.EndWrite(); // Close the file

std::vector<int> deserialised{}; // Declare the memory where we're going to put the deserialised data

s.BeginRead("data.txt"); // Open the file
s.Deserialise<int>(deserialised); // Deserialise integers into the assigned memory
s.EndRead(); // Close the file

------
Strings
------
std::string original = "original string data";

s.BeginWrite("data.txt");
s.Serialise(&original, original.size());
s.EndWrite();

std::string deserialised = "";
s.BeginRead("data.txt");
s.Deserialise(&end);
s.EndRead();

------
Arrays
------

int original[] = { 0, 1, 2, 3 };

s.BeginWrite("data.txt");
s.Serialise(original, 4, sizeof(int));
s.EndWrite();

unsigned int deserialised_length = 0;
s.BeginRead("data.txt");
int* deserialised = s.DeserialiseArray<int>(deserialised_length);
s.EndRead();
*/

class Serialiser {
private:
	std::ofstream* outstream;
	std::ifstream* instream;

public:
	// Open the file we're deserialising from
	void BeginRead(const char* path);
	// Close the file we're serialising to
	void BeginWrite(const char* path);
	// Close the file we're deserialising from
	void EndRead();
	// Close the file we're serialising to
	void EndWrite();

	// Serialise a string of "length"
	void Serialise(std::string* data, unsigned int length);
	// Serialise a list who has "length" amount of elements, where each element has "size" bytes
	void Serialise(const void* data, unsigned int length, unsigned int size);

	// Serialises a vector of type T
	template <typename T>
	void Serialise(const std::vector<T>& data) {
		unsigned int length = data.size(); // Get amount of elements

		outstream->write((char*)&length, sizeof(unsigned int)); // Write the amount of elements in the data (an unsigned int which takes 4 bytes)
		outstream->write((char*)&data[0], length * sizeof(T)); // Write all data in vector
	}

	// Serialise one object (must be POD)
	template <typename T>
	void Serialise(const T& data) {
		outstream->write((char*)&data, sizeof(T));
	}

	// Deserialises one instance of a string and stores in string pointer given
	void Deserialise(std::string* memory);
	
	// Deserialises one instance of an object type T and returns it
	template <typename T>
	T Deserialise() {
		char* buffer = new char[sizeof(T)]; // Create buffer to store binary data from file
		instream->read(buffer, sizeof(T)); // Read binary data from file

		// Create an instance of the object, so appropriate memory is allocated
		T instance;
		// Copy binary data into the memory location of the instance of the object we created
		// NOTE: this is just copying the raw data over, so some necessary processes for object initialisation may not be run (thus this should be limited to PODs)
		memcpy(&instance, buffer, sizeof(T));

		return instance;
	}

	// Deserialises an array, takes a reference to where the size of the deserialised array will be stored, and returns a pointer to the deserialised array (which is on heap)
	template <typename T>
	T* DeserialiseArray(unsigned int& _length) {
		unsigned int length = Deserialise<unsigned int>(); // Get length of array
		T* memory = new T[length]; // Allocate buffer to store deserialised data

		for (unsigned int i = 0; i < length; i++) {
			memory[i] = Deserialise<T>(); // Deserialise an instance and add to list
		}

		_length = length; // Update _amount to represent length of deserialised array
		return memory; // Return list pointer
	}

	// Deserialises a vector, takes a reference to a vector where the deserialised data will be stored
	template <typename T>
	void Deserialise(std::vector<T>& memory) {
		unsigned int length = Deserialise<unsigned int>(); // Get length of vector
		
		memory.reserve(sizeof(T) * length); // Reserve the space the vector will take up

		for (unsigned int i = 0; i < length; i++) {
			memory.push_back(Deserialise<T>()); // Deserialise an instance, and add to vector
		}
	}

	Serialiser();
	~Serialiser();
};