#include "Noise.h"

int Noise::Hash(unsigned int seed, int x) {
    // Forgot where I ripped this hashing algorithm from
    x ^= seed;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

float Noise::Hashf(unsigned int seed, int x)
{
    // Hash an integer
    unsigned int v = x ^ seed;
    v = ((v >> 16) ^ v) * 0x45d9f3b;
    v = ((v >> 16) ^ v) * 0x45d9f3b;
    v = (v >> 16) ^ v;

    // Do some bit magic to put the integer's bits into the mantissa of a float (forgot where I ripped this from as well)
    const unsigned int mantissa_bitmask = 0x007FFFFFu;
    const unsigned int one_binary_value = 0x3F800000u;

    v &= mantissa_bitmask;
    v |= one_binary_value;

    // Trick compiler into interpreting our "integer" as a float
    float f = (*(float*)&v) - 1.0f;

    return f;
}

int Noise::Hash(unsigned int seed, Vector2<int> pos)
{
    return Hash(seed, pos.x ^ Hash(seed, pos.y));
}

int Noise::Hash(unsigned int seed, int x, int y)
{
    return Hash(seed, x ^ Hash(seed, y));
}

float Noise::Hashf(unsigned int seed, Vector2<int> pos)
{
    return Hashf(seed, pos.x ^ Hash(seed, pos.y));
}

float Noise::Hashf(unsigned int seed, int x, int y)
{
    return Hashf(seed, x ^ Hash(seed, y));
}

double Noise::LinearInterpolate(double a, double b, double speed)
{
	return a + (speed * (b - a));
}

double Noise::SmoothStepInterpolate(double a, double b, double speed)
{
	float t = speed * speed * (3 - (2 * speed));
	return a + (t * (b - a));
}

uint8_t Noise::LinearInterpolate(uint8_t a, uint8_t b, double speed)
{
    return a + (speed * (b - a));
}

uint8_t Noise::SmoothStepInterpolate(uint8_t a, uint8_t b, double speed)
{
    float t = speed * speed * (3 - (2 * speed));
    return a + (t * (b - a));
}

float Noise::White::Get(int x)
{
    return Hashf(m_seed, x / wavelength) * amplitude;
}

float Noise::White::Get(int x, int y)
{
    return Hashf(m_seed, Vector2<int>(x, y) / wavelength) * amplitude;
}

float* Noise::White::GetList(int x, unsigned int length)
{
    float* values = new float[length];

    for (int i = 0; i < length; i++) {
        values[i] = Get(m_seed, x);
    }

    return values;
}

float** Noise::White::GetList(int x, int y, unsigned int width, unsigned int height)
{
    float** values = new float* [height];

    for (int j = 0; j < height; j++) {
        float* row = new float[width];

        for (int i = 0; i < width; x++) {
            row[i] = Get(m_seed, i + x);
        }

        values[j] = row;
    }

    return values;
}

uint8_t Noise::White::GetByte(int x)
{
    return (Hash(m_seed, x / wavelength) & 0xff) * amplitude;
}

uint8_t Noise::White::GetByte(int x, int y)
{
    return (Hash(m_seed, Vector2<int>(x, y) / wavelength) & 0xff) * amplitude;
}

uint8_t* Noise::White::GetByteList(int x, unsigned int length)
{
    uint8_t* values = new uint8_t[length];

    for (int i = 0; i < length; i++) {
        values[i] = GetByte(m_seed, i + x);
    }

    return values;
}

uint8_t** Noise::White::GetByteList(int x, int y, unsigned int width, unsigned int height)
{
    uint8_t** values = new uint8_t*[height]; // 2D list of unsigned bytes

    for (int j = 0; j < height; j++) {
        uint8_t* row = new uint8_t[width];

        for (int i = 0; i < width; x++) {
            row[i] = GetByte(m_seed, i + x);
        }

        values[j] = row;
    }

    return values;
}

Noise::White::White(unsigned int seed, float amplitude, unsigned int wavelength)
    : m_seed(seed), amplitude(amplitude), wavelength(wavelength) {}

Noise::White::~White() {}

float Noise::Interpolated::GetLinear(int x)
{
    float current = Hashf(m_seed, x);
    float next = Hashf(m_seed, x + 1);

    // Set speed based on distance from previous random point to next random point
    float speed = float((x / wavelength) & wavelength) / float(wavelength);

    return LinearInterpolate(current, next, speed);
}

float Noise::Interpolated::GetSmooth(int x)
{
    float current = Hashf(m_seed, x);
    float next = Hashf(m_seed, x + 1);

    // Set speed based on distance from previous random point to next random point
    float speed = float((x / wavelength) & wavelength) / float(wavelength);

    return SmoothStepInterpolate(current, next, speed);
}

uint8_t Noise::Interpolated::GetByteLinear(int x)
{
    uint8_t current = Hash(m_seed, x) & 0xff;
    uint8_t next = Hash(m_seed, x + 1) & 0xff;

    // Set speed based on distance from previous random point to next random point
    float speed = float((x / wavelength) & wavelength) / float(wavelength);

    return LinearInterpolate(current, next, speed);
}

uint8_t Noise::Interpolated::GetByteSmooth(int x)
{
    uint8_t current = Hash(m_seed, x) & 0xff;
    uint8_t next = Hash(m_seed, x + 1) & 0xff;

    // Set speed based on distance from previous random point to next random point
    float speed = float((x / wavelength) & wavelength) / float(wavelength);

    return SmoothStepInterpolate(current, next, speed);
}

Noise::Interpolated::Interpolated(unsigned int seed, float amplitude, unsigned int wavelength)
    : m_seed(seed), amplitude(amplitude), wavelength(wavelength) {}

Noise::Interpolated::~Interpolated() {}
