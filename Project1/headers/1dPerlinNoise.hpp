#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <random>
#include <algorithm>

class PerlinNoise1D
{
private:
    // Doubled size (512) prevents wrapping bugs during lookup
    int p[512];

    // Inline allows the compiler to replace the function call with raw math
    inline double fade(double t) noexcept {
        return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
    }

    inline double lerp(double t, double a, double b) noexcept {
        return a + t * (b - a);
    }

    inline double getGradient(int integerX) noexcept {
        // Safe bitwise masking for negative coordinates
        int hash = p[integerX & 255];
        return (hash & 1) ? 1.0 : -1.0;
    }

public:
    PerlinNoise1D(unsigned int seed)
    {
        std::vector<int> temp(256);
        std::iota(temp.begin(), temp.end(), 0);

        std::mt19937 engine(seed);
        std::shuffle(temp.begin(), temp.end(), engine);

        // Duplicate the array to eliminate boundary checking overhead
        for (int i = 0; i < 256; ++i) {
            p[i] = temp[i];
            p[i + 256] = temp[i];
        }
    }

    double noise(double x) noexcept
    {
        // Fast floor calculation
        int X0 = static_cast<int>(std::floor(x));

        // Calculate relative distance
        double dist0 = x - X0;
        double dist1 = dist0 - 1.0;

        // Gradient lookups
        double grad0 = getGradient(X0);
        double grad1 = getGradient(X0 + 1);

        // Core Perlin math
        double influence0 = grad0 * dist0;
        double influence1 = grad1 * dist1;

        double u = fade(dist0);

        // Map output cleanly from [-0.5, 0.5] to [0.0, 1.0]
        return lerp(u, influence0, influence1) + 0.5;
    }

    // NEW: Multi-layered fractal noise function
    double fractalNoise(double x, int octaves, double lacunarity, double gain) noexcept
    {
        double total = 0.0;
        double amplitude = 1.0;
        double frequency = 1.0;
        double maxValue = 0.0; // Used for normalizing the final result

        for (int i = 0; i < octaves; ++i) {
            // Get noise sample and shift it back to [-0.5, 0.5] for proper layering
            double noiseVal = noise(x * frequency) - 0.5;

            total += noiseVal * amplitude;
            maxValue += amplitude;

            frequency *= lacunarity; // Increase detail level
            amplitude *= gain;       // Decrease impact of finer details
        }

        // Normalize the final accumulated values back to [0.0, 1.0] range
        return (total / maxValue) + 0.5;
    }
};


    //unsigned int seed = 42; // You can change this seed for different noise patterns
    //PerlinNoise1D perlin(seed);
    //std::cout << "Seed: "<<seed<<"\n";

    //std::cout << "\nFractal Noise:\n";

    //int octaves = 10;
    //double lacunarity = 2.0;
    //double gain = 0.5;

    //for (double x = 0.0; x < 10.0; x += 0.1)
    //{
    //	double fractalnoiseValue = perlin.fractalNoise(x, octaves, lacunarity, gain);
    //	//std::cout << "Fractal noise at " << x << ": " << fractalnoiseValue << std::endl;
    //	int spaces = static_cast<int>(fractalnoiseValue * 30); // Scale for visualization

    //	for (int i = 0; i < spaces; ++i)
    //	{
    //		std::cout << " ";
    //	}
    //	std::cout << "*" << std::endl; // Represent the noise value with a star
    //}
