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

Vector2<int>ENGINE_API Noise::Hash2(unsigned int seed, Vector2<int> pos)
{
    return Vector2<int>(Hash(seed, pos.x), Hash(seed, pos.y));
}

Vector2<float>ENGINE_API Noise::Hash2f(unsigned int seed, Vector2<int> pos)
{
    return Vector2<float>(Hashf(seed, pos.x), Hashf(seed, pos.y));
}

float Noise::Interpolate(float a, float b, float t)
{
	return a + (t * (b - a));
}

float Noise::Cubic(float t)
{
    return t * t * (3 - (2 * t));
}

// Gives the speed of interpolation
float Noise::Quintic(float t)
{
    return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
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

Noise::White::White(unsigned int seed, float amplitude, int wavelength)
{
    this->m_seed = seed;
    this->amplitude = amplitude;
    this->wavelength = wavelength;
}

Noise::White::~White() {}

float Noise::Interpolated::GetLinear(int x)
{
    float current = Hashf(m_seed, x / wavelength);
    float next = Hashf(m_seed, (x + 1) / wavelength);

    // Set speed based on distance from previous random point to next random point
    float speed = float((x / wavelength) & wavelength) / float(wavelength);

    return Interpolate(current, next, speed);
}

float Noise::Interpolated::GetSmooth(int x)
{
    float current = Hashf(m_seed, x / wavelength);
    float next = Hashf(m_seed, (x + 1) / wavelength);

    // Set speed based on distance from previous random point to next random point
    float speed = float((x / wavelength) & wavelength) / float(wavelength);

    return Interpolate(current, next, Cubic(speed));
}

uint8_t Noise::Interpolated::GetByteLinear(int x)
{
    return GetLinear(x) * 255;
}

uint8_t Noise::Interpolated::GetByteSmooth(int x)
{
    return GetSmooth(x) * 255;
}

Noise::Interpolated::Interpolated(unsigned int seed, float amplitude, int wavelength)
{
    this->m_seed = seed;
    this->amplitude = amplitude;
    this->wavelength = wavelength;
}

Noise::Interpolated::~Interpolated() {}

void Noise::__NoiseVirtual::SetSeed(unsigned int nseed)
{
    m_seed = nseed;
}

Noise::__NoiseVirtual::__NoiseVirtual()
    : m_seed(0), amplitude(1), wavelength(1)
{}

Noise::__NoiseVirtual::__NoiseVirtual(unsigned int m_seed, float amplitude, int wavelength)
    : m_seed(m_seed), amplitude(amplitude), wavelength(wavelength)
{}

Noise::__NoiseVirtual::~__NoiseVirtual() {}

float Noise::Interpolated::Get(int x, int y)
{
    Vector2<float> p = Vector2<float>(x, y) / (float)wavelength; // Gives a decimal value and is the "real" coordinate of our point

    // Calculate surrounding points/lattice
    Vector2<float> l0 = p.floor();
    Vector2<float> l1 = p.floor() + Vector2<float>(1.0f, 0.0f);
    Vector2<float> l2 = p.floor() + Vector2<float>(0.0f, 1.0f);
    Vector2<float> l3 = p.floor() + Vector2<float>(1.0f, 1.0f);

    // Calculate gradients for each lattice point
    float r0 = Hashf(m_seed, l0);
    float r1 = Hashf(m_seed, l1);
    float r2 = Hashf(m_seed, l2);
    float r3 = Hashf(m_seed, l3);

    // Calculate interpolation speeds for x and y
    float tx = Quintic(p.x - l0.x);
    float ty = Quintic(p.y - l0.y);

    // Interpolate for each x slice
    float p0p1 = Interpolate(r0, r1, tx);
    float p2p3 = Interpolate(r2, r3, tx);

    // Interpolate between each x slice
    return Interpolate(p0p1, p2p3, ty) * amplitude;
}

float Noise::Interpolated::GetFractal(int x, int y, int octaves)
{
    constexpr float SCALE = 0.5f;

    float original_amp = amplitude;
    int original_wl = wavelength;

    float v = 0.0;
    float total_amplitude = 0.0;

    for (int i = 0; i < octaves; i++) {
        v += Get(x, y);

        total_amplitude += amplitude;
        amplitude *= SCALE;
        wavelength *= SCALE;
    }

    amplitude = original_amp;
    wavelength = original_wl;

    return v / total_amplitude;
}