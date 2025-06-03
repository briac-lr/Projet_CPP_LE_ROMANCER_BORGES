#pragma once

#include<random>
#include <corecrt_math_defines.h>
#include <cmath>

class StandardGaussianGenerator {
public:
    // Constructor
    StandardGaussianGenerator()
        : _eng(_rd()), _dist(0., 1.) {}

    // Generate a random number following the standard normal distribution
    inline double generate() const {
        return _dist(_eng);
    }

    static inline double cdf(double x) {
        return 0.5 * erfc(-x * M_SQRT1_2);
    }

private:
    // Non-deterministic random number generator
    std::random_device _rd;

    // Mersenne Twister pseudo-random generator
    mutable std::mt19937 _eng;

    // Normal distribution
    mutable std::normal_distribution<double> _dist;
};