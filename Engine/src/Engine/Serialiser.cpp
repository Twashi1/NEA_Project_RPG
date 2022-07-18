#include "Serialiser.h"
#include "Vector2.h"
#include "Vector3.h"

namespace Serialiser {
	void Stream::BeginRead(const char* path)
	{
		// If instream is not a nullptr, the instream hasn't been closed yet
		if (in != nullptr) ENG_LogWarn("Serialiser file not closed before opening new one");
		in = new std::ifstream(path, std::ios::binary);
	}

	void Stream::BeginWrite(const char* path)
	{
		// If outstream is not a nullptr, the outstream hasn't been closedyet
		if (out != nullptr) ENG_LogWarn("Serialiser file not closed before opening new one");
		out = new std::ofstream(path, std::ios::binary | std::ofstream::trunc);
	}

	void Stream::EndRead() { in->close(); delete in; in = nullptr; }
	void Stream::EndWrite() { out->close(); delete out; out = nullptr; }

	Stream::Stream() : in(nullptr), out(nullptr) {}

	Stream::~Stream() {
		if (in != nullptr) delete in;
		if (out != nullptr) delete out;
	}

	template <__Streamable T> void Serialise(Stream& s, const T& data)
	{
		s.out->write((char*)&data, sizeof(T));
	}

	template ENGINE_API void Serialise(Stream&, const int8_t&);
	template ENGINE_API void Serialise(Stream&, const uint8_t&);
	template ENGINE_API void Serialise(Stream&, const int16_t&);
	template ENGINE_API void Serialise(Stream&, const uint16_t&);
	template ENGINE_API void Serialise(Stream&, const int32_t&);
	template ENGINE_API void Serialise(Stream&, const uint32_t&);
	template ENGINE_API void Serialise(Stream&, const long&);
	template ENGINE_API void Serialise(Stream&, const float&);
	template ENGINE_API void Serialise(Stream&, const double&);

	template <> void Serialise<std::string>(Stream& s, const std::string& data)
	{
		s.out->write(data.c_str(), data.length());
		s.out->put('\0'); // Write null termination character
	}

	template <__Streamable T> T Unserialise(Stream& s)
	{
		// T must have default constructor
		ENG_Assert(std::is_default_constructible<T>::value, "Type {} is not default constructible", typeid(T).name());

		char* read_buffer = new char[sizeof(T)]; // Allocate buffer for file data
		s.in->read(read_buffer, sizeof(T)); // Read file data into buffer

		T instance;
		memcpy(&instance, read_buffer, sizeof(T)); // Copy data from buffer into pointer given

		delete[] read_buffer; // Delete buffer after copying the data

		return instance;
	}

	template ENGINE_API int8_t			Unserialise(Stream&);
	template ENGINE_API uint8_t			Unserialise(Stream&);
	template ENGINE_API int16_t			Unserialise(Stream&);
	template ENGINE_API uint16_t		Unserialise(Stream&);
	template ENGINE_API int32_t			Unserialise(Stream&);
	template ENGINE_API uint32_t		Unserialise(Stream&);
	template ENGINE_API long			Unserialise(Stream&);
	template ENGINE_API float			Unserialise(Stream&);
	template ENGINE_API double			Unserialise(Stream&);

	template <> std::string Unserialise<std::string>(Stream& s)
	{
		std::string buffer = "";
		char lastchar;

		s.in->get(lastchar); // Get first character
		buffer += lastchar; // Add to buffer

		// While the last character is not null termination character
		while (lastchar != '\0') {
			s.in->get(lastchar); // Get next character
			buffer += lastchar; // Add next character to buffer

			// If character is null termination character, break
			if (lastchar == '\0') break;
		}

		// Copy buffer to memory 
		return buffer;
	}

	template <__Streamable T> void Unserialise(Stream& s, T* memory)
	{
		char* readbuff = new char[sizeof(T)]; // Allocate buffer for file data
		s.in->read(readbuff, sizeof(T)); // Read file data into buffer

		memcpy(memory, readbuff, sizeof(T)); // Copy data from buffer into pointer given

		delete[] readbuff; // Delete buffer after copying the data
	}

	template ENGINE_API void Unserialise(Stream&, int8_t*);
	template ENGINE_API void Unserialise(Stream&, uint8_t*);
	template ENGINE_API void Unserialise(Stream&, int16_t*);
	template ENGINE_API void Unserialise(Stream&, uint16_t*);
	template ENGINE_API void Unserialise(Stream&, int32_t*);
	template ENGINE_API void Unserialise(Stream&, uint32_t*);
	template ENGINE_API void Unserialise(Stream&, long*);
	template ENGINE_API void Unserialise(Stream&, float*);
	template ENGINE_API void Unserialise(Stream&, double*);

	template <> void Unserialise<std::string>(Stream& s, std::string* memory)
	{
		char lastchar;

		s.in->get(lastchar); // Get first character
		*memory += lastchar; // Add to memory

		// While the last character is not null termination character
		while (lastchar != '\0') {
			s.in->get(lastchar); // Get next character
			*memory += lastchar; // Add next character to memory

			// If character is null termination character, break
			if (lastchar == '\0') break;
		}
	}

	Streamable::Streamable() {}
	Streamable::~Streamable() {}
	void Streamable::Write(Stream& s) { ENG_LogInfo("Write streamable"); }
	void Streamable::Read(Stream& s) { ENG_LogInfo("Read streamable"); }
}