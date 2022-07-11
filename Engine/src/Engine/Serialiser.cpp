#include "Serialiser.h"

void Serialiser::BeginRead(const char* path)
{
	// If instream is not a nullptr, the instream hasn't been closed yet
	if (instream != nullptr) Log("Serialiser file not closed before opening new one", LOG::WARNING);
	instream = new std::ifstream(path, std::ios::binary);
}

void Serialiser::BeginWrite(const char* path)
{
	// If outstream is not a nullptr, the outstream hasn't been closedyet
	if (outstream != nullptr) Log("Serialiser file not closed before opening new one", LOG::WARNING);
	outstream = new std::ofstream(path, std::ios::binary | std::ofstream::trunc);
}

void Serialiser::EndRead() { instream->close(); delete instream; instream = nullptr; }
void Serialiser::EndWrite() { outstream->close(); delete outstream; outstream = nullptr; }

Serialiser::Serialiser() : instream(nullptr), outstream(nullptr) {}

Serialiser::~Serialiser() {
	if (instream != nullptr) delete instream;
	if (outstream != nullptr) delete outstream;
}

template <typename T>
void Serialise(Serialiser& s, const T& data)
{
	s.outstream->write((char*)&data, sizeof(T));
}

template ENGINE_API void Serialise(Serialiser&, const int&);
template ENGINE_API void Serialise(Serialiser&, const uint8_t&);
template ENGINE_API void Serialise(Serialiser&, const unsigned char&);
template ENGINE_API void Serialise(Serialiser&, const unsigned int&);
template ENGINE_API void Serialise(Serialiser&, const double&);
template ENGINE_API void Serialise(Serialiser&, const float&);
template ENGINE_API void Serialise(Serialiser&, const uint16_t&);
template ENGINE_API void Serialise(Serialiser&, const unsigned short&);
template ENGINE_API void Serialise(Serialiser&, const Vector2<float>&);
template ENGINE_API void Serialise(Serialiser&, const Vector2<int>&);
template ENGINE_API void Serialise(Serialiser&, const Vector2<uint8_t>&);

template <> void Serialise<std::string>(Serialiser& s, const std::string& data)
{
	s.outstream->write(data.c_str(), data.length());
	s.outstream->put('\0'); // Write null termination character
}

template <> void Serialise<VersionNumber>(Serialiser& s, const VersionNumber& v)
{
	Serialise<uint16_t>(s, v.major);
	Serialise<uint16_t>(s, v.minor);
	Serialise<uint16_t>(s, v.patch);
}

template<> void Serialise(Serialiser& s, const Rect& data)
{
	Serialise<float>(s, data.x);
	Serialise<float>(s, data.y);
	Serialise<float>(s, data.width);
	Serialise<float>(s, data.height);
	Serialise<float>(s, data.angle);
}

template<> void Serialise(Serialiser& s, const Quad& data)
{
	Serialise<Rect>(s, data.GetRect());
}

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

template ENGINE_API void Serialise(Serialiser&, const std::vector<int>&);
template ENGINE_API void Serialise(Serialiser&, const std::vector<uint8_t>&);
template ENGINE_API void Serialise(Serialiser&, const std::vector<uint16_t>&);
template ENGINE_API void Serialise(Serialiser&, const std::vector<uint32_t>&);
template ENGINE_API void Serialise(Serialiser&, const std::vector<float>&);
template ENGINE_API void Serialise(Serialiser&, const std::vector<double>&);
template ENGINE_API void Serialise(Serialiser&, const std::vector<unsigned int>&);

template <typename T>
void Serialise(Serialiser& s, T* data, const uint32_t& length)
{
	Serialise<T>(s, length); // Serialise length of list

	// Serialise each element in array
	for (uint32_t i = 0; i < length; i++) {
		Serialise<T>(s, data[i]);
	}
}

template ENGINE_API void Serialise(Serialiser&, int*, const uint32_t&);
template ENGINE_API void Serialise(Serialiser&, uint8_t*, const uint32_t&);
template ENGINE_API void Serialise(Serialiser&, float*, const uint32_t&);
template ENGINE_API void Serialise(Serialiser&, double*, const uint32_t&);
template ENGINE_API void Serialise(Serialiser&, unsigned int*, const uint32_t&);
template ENGINE_API void Serialise(Serialiser&, uint32_t*, const uint32_t&);

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

template ENGINE_API int Deserialise(const Serialiser&);
template ENGINE_API uint8_t Deserialise(const Serialiser&);
template ENGINE_API float Deserialise(const Serialiser&);
template ENGINE_API double Deserialise(const Serialiser&);
template ENGINE_API unsigned int Deserialise(const Serialiser&);
template ENGINE_API Vector2<float> Deserialise(const Serialiser&);
template ENGINE_API Vector2<int> Deserialise(const Serialiser&);

template <typename T>
void Deserialise(const Serialiser& s, T* memory)
{
	char* readbuff = new char[sizeof(T)]; // Allocate buffer for file data
	s.instream->read(readbuff, sizeof(T)); // Read file data into buffer

	memcpy(memory, readbuff, sizeof(T)); // Copy data from buffer into pointer given

	delete[] readbuff; // Delete buffer after copying the data
}

template ENGINE_API void Deserialise(const Serialiser&, int*);
template ENGINE_API void Deserialise(const Serialiser&, uint8_t*);
template ENGINE_API void Deserialise(const Serialiser&, uint16_t*);
template ENGINE_API void Deserialise(const Serialiser&, float*);
template ENGINE_API void Deserialise(const Serialiser&, double*);
template ENGINE_API void Deserialise(const Serialiser&, unsigned int*);
template ENGINE_API void Deserialise(const Serialiser&, Vector2<float>*);
template ENGINE_API void Deserialise(const Serialiser&, Vector2<int>*);

template <> void Deserialise<std::string>(const Serialiser& s, std::string* memory)
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

template<> void Deserialise(const Serialiser& s, Rect* memory)
{
	float x, y, width, height, angle;
	Deserialise<float>(s, &x);
	Deserialise<float>(s, &y);
	Deserialise<float>(s, &width);
	Deserialise<float>(s, &height);
	Deserialise<float>(s, &angle);

	*memory = Rect(x, y, width, height, angle);
}

template<> void Deserialise(const Serialiser& s, Quad* memory)
{
	Rect rect;
	Deserialise<Rect>(s, &rect);

	*memory = Quad(Rect(rect)); // Using a copy constructor to ensure everything is being properly initialised
}

template <> void Deserialise<VersionNumber>(const Serialiser& s, VersionNumber* memory)
{
	uint16_t major, minor, patch;
	Deserialise<uint16_t>(s, &major);
	Deserialise<uint16_t>(s, &minor);
	Deserialise<uint16_t>(s, &patch);

	*memory = VersionNumber(major, minor, patch);
}

template <typename T>
void Deserialise(const Serialiser& s, std::vector<T>* memory)
{
	uint32_t length = Deserialise<uint32_t>(s); // Get length of vector
	memory->reserve(length); // Reserve space needed for vector

	for (uint32_t i = 0; i < length; i++) {
		memory->push_back(Deserialise<T>(s)); // Deserialise and push to vector
	}
}

template ENGINE_API void Deserialise(const Serialiser&, std::vector<int>*);
template ENGINE_API void Deserialise(const Serialiser&, std::vector<uint8_t>*);
template ENGINE_API void Deserialise(const Serialiser&, std::vector<float>*);
template ENGINE_API void Deserialise(const Serialiser&, std::vector<double>*);
template ENGINE_API void Deserialise(const Serialiser&, std::vector<unsigned int>*);

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

template ENGINE_API int* DeserialiseArray(const Serialiser&, uint32_t*);
template ENGINE_API uint8_t* DeserialiseArray(const Serialiser&, uint32_t*);
template ENGINE_API float* DeserialiseArray(const Serialiser&, uint32_t*);
template ENGINE_API double* DeserialiseArray(const Serialiser&, uint32_t*);
template ENGINE_API unsigned int* DeserialiseArray(const Serialiser&, uint32_t*);

template <typename T>
ENGINE_API void DeserialiseArray(const Serialiser& s, T* memory)
{
	// Get length of array
	uint32_t length;
	Deserialise<uint32_t>(s, &length);

	for (uint32_t i = 0; i < length; i++) {
		// Deserialise each object
		Deserialise<T>(s, &(memory[i]));
	}
}

template ENGINE_API void DeserialiseArray(const Serialiser&, int*);
template ENGINE_API void DeserialiseArray(const Serialiser&, uint8_t*);
template ENGINE_API void DeserialiseArray(const Serialiser&, uint16_t*);
template ENGINE_API void DeserialiseArray(const Serialiser&, unsigned int*);
template ENGINE_API void DeserialiseArray(const Serialiser&, float*);
template ENGINE_API void DeserialiseArray(const Serialiser&, double*);
template ENGINE_API void DeserialiseArray(const Serialiser&, Vector2<float>*);
template ENGINE_API void DeserialiseArray(const Serialiser&, Vector2<double>*);
template ENGINE_API void DeserialiseArray(const Serialiser&, std::string*);
