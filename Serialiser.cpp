#include "Serialiser.h"

void Serialiser::BeginRead(const char* path) { instream = new std::ifstream(path, std::ios::binary); }
void Serialiser::BeginWrite(const char* path) { outstream = new std::ofstream(path, std::ios::binary | std::ofstream::trunc); }
void Serialiser::EndRead() { instream->close(); delete instream; instream = nullptr; }
void Serialiser::EndWrite() { outstream->close(); delete outstream; outstream = nullptr; }

void Serialiser::Serialise(std::string* data, unsigned int length)
{
	outstream->write((char*)&length, sizeof(unsigned int)); // Assign 4 bytes to represent length of data
	outstream->write(data->c_str(), length); // Write the characters in
}

void Serialiser::Serialise(const void* data, unsigned int length, unsigned int size) {
	outstream->write((char*)&length, sizeof(unsigned int)); // Assign 4 bytes to represent length of data
	outstream->write((char*)data, length * size); // Write all the raw data in
}

void Serialiser::Deserialise(std::string* memory) {
	unsigned int length = Deserialise<unsigned int>(); // Get length of string

	// Read file data into buffer
	char* buffer = new char[length + 1]; // +1 for null termination character
	instream->read(buffer, length); // Read data into buffer
	buffer[length] = '\0'; // Set last character to null termination

	*memory = reinterpret_cast<const char*>(buffer); // Convert buffer data into cstr, and copy into memory given

	delete[] buffer; // Free buffer
}

Serialiser::Serialiser() : instream(nullptr), outstream(nullptr) {}

Serialiser::~Serialiser() {
	if (instream != nullptr) delete instream;
	if (outstream != nullptr) delete outstream;
}