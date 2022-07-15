#include "Serialiser.h"
#include "Vector2.h"
#include "Vector3.h"

void Serialiser::BeginRead(const char* path)
{
	// If instream is not a nullptr, the instream hasn't been closed yet
	if (instream != nullptr) ENG_LogWarn("Serialiser file not closed before opening new one");
	instream = new std::ifstream(path, std::ios::binary);
}

void Serialiser::BeginWrite(const char* path)
{
	// If outstream is not a nullptr, the outstream hasn't been closedyet
	if (outstream != nullptr) ENG_LogWarn("Serialiser file not closed before opening new one");
	outstream = new std::ofstream(path, std::ios::binary | std::ofstream::trunc);
}

void Serialiser::EndRead() { instream->close(); delete instream; instream = nullptr; }
void Serialiser::EndWrite() { outstream->close(); delete outstream; outstream = nullptr; }

Serialiser::Serialiser() : instream(nullptr), outstream(nullptr) {}

Serialiser::~Serialiser() {
	if (instream != nullptr) delete instream;
	if (outstream != nullptr) delete outstream;
}

void Serialiseable::Load(Serialiser& s) { ENG_LogWarn("Serialiseable base function called - have you defined an override for Load(Serialiser& s)?"); };
void Serialiseable::Unload(Serialiser& s) const { ENG_LogWarn("Serialiseable base function called - have you defined an override for Unload(Deserialiser& s)?"); };

template <typename T> void Serialise(Serialiser& s, const T& data)
{
	s.outstream->write((char*)&data, sizeof(T));
}

template ENGINE_API void Serialise(Serialiser&, const int8_t&);
template ENGINE_API void Serialise(Serialiser&, const uint8_t&);
template ENGINE_API void Serialise(Serialiser&, const int16_t&);
template ENGINE_API void Serialise(Serialiser&, const uint16_t&);
template ENGINE_API void Serialise(Serialiser&, const int&);
template ENGINE_API void Serialise(Serialiser&, const uint32_t&);
template ENGINE_API void Serialise(Serialiser&, const float&);
template ENGINE_API void Serialise(Serialiser&, const double&);

template <> void Serialise<std::string>(Serialiser& s, const std::string& data)
{
	s.outstream->write(data.c_str(), data.length());
	s.outstream->put('\0'); // Write null termination character
}

template <typename T> T Deserialise(Serialiser& s)
{
	// T must have default constructor
	ENG_Assert(std::is_default_constructible<T>::value, "Type {} is not default constructible", typeid(T).name());

	char* read_buffer = new char[sizeof(T)]; // Allocate buffer for file data
	s.instream->read(read_buffer, sizeof(T)); // Read file data into buffer

	T instance;
	memcpy(&instance, read_buffer, sizeof(T)); // Copy data from buffer into pointer given

	delete[] read_buffer; // Delete buffer after copying the data

	return instance;
}

template ENGINE_API int8_t			Deserialise(Serialiser&);
template ENGINE_API uint8_t			Deserialise(Serialiser&);
template ENGINE_API int16_t			Deserialise(Serialiser&);
template ENGINE_API uint16_t		Deserialise(Serialiser&);
template ENGINE_API int				Deserialise(Serialiser&);
template ENGINE_API uint32_t		Deserialise(Serialiser&);
template ENGINE_API float			Deserialise(Serialiser&);
template ENGINE_API double			Deserialise(Serialiser&);

template <> std::string Deserialise<std::string>(Serialiser& s)
{
	std::string buffer = "";
	char lastchar;

	s.instream->get(lastchar); // Get first character
	buffer += lastchar; // Add to buffer

	// While the last character is not null termination character
	while (lastchar != '\0') {
		s.instream->get(lastchar); // Get next character
		buffer += lastchar; // Add next character to buffer

		// If character is null termination character, break
		if (lastchar == '\0') break;
	}

	// Copy buffer to memory 
	return buffer;
}

template <typename T> void Deserialise(Serialiser& s, T* memory)
{
	char* readbuff = new char[sizeof(T)]; // Allocate buffer for file data
	s.instream->read(readbuff, sizeof(T)); // Read file data into buffer

	memcpy(memory, readbuff, sizeof(T)); // Copy data from buffer into pointer given

	delete[] readbuff; // Delete buffer after copying the data
}

template ENGINE_API void Deserialise(Serialiser&, int8_t*);
template ENGINE_API void Deserialise(Serialiser&, uint8_t*);
template ENGINE_API void Deserialise(Serialiser&, int16_t*);
template ENGINE_API void Deserialise(Serialiser&, uint16_t*);
template ENGINE_API void Deserialise(Serialiser&, int*);
template ENGINE_API void Deserialise(Serialiser&, uint32_t*);
template ENGINE_API void Deserialise(Serialiser&, float*);
template ENGINE_API void Deserialise(Serialiser&, double*);

template <> void Deserialise<std::string>(Serialiser& s, std::string* memory)
{
	char lastchar;

	s.instream->get(lastchar); // Get first character
	*memory += lastchar; // Add to memory

	// While the last character is not null termination character
	while (lastchar != '\0') {
		s.instream->get(lastchar); // Get next character
		*memory += lastchar; // Add next character to memory

		// If character is null termination character, break
		if (lastchar == '\0') break;
	}
}