#ifndef UTILS_HPP
#define UTILS_HPP

#include <cmath>
#include <iostream>
#include <ctime>

namespace utils {
    inline double log(double x) { return std::log(x); }
    inline double normal_dist(double mean, double stddev) {
        // Простая заглушка для нормального распределения (можно улучшить с Boost)
        return mean + stddev * (std::rand() / (RAND_MAX + 1.0) - 0.5);
    }
}

#endif