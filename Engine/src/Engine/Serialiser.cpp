#include "Serialiser.h"

void Serialiser::BeginRead(const char* path) { instream = new std::ifstream(path, std::ios::binary); }
void Serialiser::BeginWrite(const char* path) { outstream = new std::ofstream(path, std::ios::binary | std::ofstream::trunc); }
void Serialiser::EndRead() { instream->close(); delete instream; instream = nullptr; }
void Serialiser::EndWrite() { outstream->close(); delete outstream; outstream = nullptr; }

Serialiser::Serialiser() : instream(nullptr), outstream(nullptr) {}

Serialiser::~Serialiser() {
	if (instream != nullptr) delete instream;
	if (outstream != nullptr) delete outstream;
}

template <>
void Deserialise<std::string>(const Serialiser& s, std::string* memory)
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
	*memory = buffer;
}

template<>
void Deserialise(const Serialiser& s, Rect* memory)
{
	float x, y, width, height, angle;
	Deserialise<float>(s, &x);
	Deserialise<float>(s, &y);
	Deserialise<float>(s, &width);
	Deserialise<float>(s, &height);
	Deserialise<float>(s, &angle);

	*memory = Rect(x, y, width, height, angle);
}

template<>
void Deserialise(const Serialiser& s, Quad* memory)
{
	Rect rect;
	Deserialise<Rect>(s, &rect);

	*memory = Quad(Rect(rect)); // Using a copy constructor to ensure everything is being properly initialised
}

template <>
void Deserialise<VersionNumber>(const Serialiser& s, VersionNumber* memory)
{
	uint16_t major, minor, patch;
	Deserialise<uint16_t>(s, &major);
	Deserialise<uint16_t>(s, &minor);
	Deserialise<uint16_t>(s, &patch);

	*memory = VersionNumber(major, minor, patch);
}

template <>
void Serialise<std::string>(Serialiser& s, const std::string& data)
{
	s.outstream->write(data.c_str(), data.length());
	s.outstream->put('\0'); // Write null termination character
}

template <>
void Serialise<VersionNumber>(Serialiser& s, const VersionNumber& v)
{
	Serialise<uint16_t>(s, v.major);
	Serialise<uint16_t>(s, v.minor);
	Serialise<uint16_t>(s, v.patch);
}

template<>
void Serialise(Serialiser& s, const Rect& data)
{
	Serialise<float>(s, data.GetX());
	Serialise<float>(s, data.GetY());
	Serialise<float>(s, data.GetWidth());
	Serialise<float>(s, data.GetHeight());
	Serialise<float>(s, data.GetAngle());
}

template<>
void Serialise(Serialiser& s, const Quad& data)
{
	Serialise<Rect>(s, data.GetRect());
}
