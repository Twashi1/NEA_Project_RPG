#pragma once

#include "Utilities.h"
#include "Vector2.h"

namespace Noise {
    // Hash integer into integer
    int ENGINE_API Hash(unsigned int seed, int x);
    // Hash integer into float
    float ENGINE_API Hashf(unsigned int seed, int x);

    // Hash 2 integers to integer
    int ENGINE_API Hash(unsigned int seed, Vector2<int> pos);
    // Hash 2 integers to integer
    int ENGINE_API Hash(unsigned int seed, int x, int y);
    // Hash 2 integers to float
    float ENGINE_API Hashf(unsigned int seed, Vector2<int> pos);
    // Hash 2 integers to float
    float ENGINE_API Hashf(unsigned int seed, int x, int y);

    // Linearly interpolate between two doubles
    double ENGINE_API LinearInterpolate(double a, double b, double speed);
    // Interpolate between two doubles using smooth step function
    double ENGINE_API SmoothStepInterpolate(double a, double b, double speed);

    // Linearly interpolate between two unsigned bytes
    uint8_t ENGINE_API LinearInterpolate(uint8_t a, uint8_t b, double speed);
    // Interpolate between two unsigned bytes using smooth step function
    uint8_t ENGINE_API SmoothStepInterpolate(uint8_t a, uint8_t b, double speed);

    class ENGINE_API White {
    private:
        unsigned int m_seed;

    public:
        float amplitude;         // Multiplier for value of noise
        unsigned int wavelength; // Determines period at which noise repeats

        // Get value noise for 1D coordinate as float
        float Get(int x);
        // Get value noise for 2D coordinate as float
        float Get(int x, int y);
        // Returns array of "length" of random float values
        float* GetList(int x, unsigned int length);
        // Returns 2D array of dimensions (width, height) of random float values (indexed [y][x])
        float** GetList(int x, int y, unsigned int width, unsigned int height);

        // Get value noise for 1D coordinate as byte
        uint8_t GetByte(int x);
        // Get value noise for 2D coordinate as byte
        uint8_t GetByte(int x, int y);
        // Returns array of "length" of random bytes
        uint8_t* GetByteList(int x, unsigned int length);
        // Returns 2D array of dimensions (width, height) of random bytes (indexed [y][x])
        uint8_t** GetByteList(int x, int y, unsigned int width, unsigned int height);

        White(unsigned int seed, float amplitude, unsigned int wavelength);
        ~White();
    };

    class ENGINE_API Interpolated {
    private:
        unsigned int m_seed;

    public:
        float amplitude;         // Multiplier for value of noise
        unsigned int wavelength; // Determines period at which noise repeats

        // Get linearly interpolated noise for 1D coordinate as float
        float GetLinear(int x);
        // Get smoothly interpolated noise for 1D coordinate as float
        float GetSmooth(int x);

        // Get linearly interpolated noise for 1D coordinate as byte
        uint8_t GetByteLinear(int x);
        // Get smoothly interpolated noise for 1D coordinate as byte
        uint8_t GetByteSmooth(int x);

        Interpolated(unsigned int seed, float amplitude, unsigned int wavelength);
        ~Interpolated();
    };
}
