#ifndef UTILS_HPP
#define UTILS_HPP

#include <cmath>
#include <iostream>
#include <ctime>
#include <random>

namespace utils {
    inline double log(double x) { return std::log(x); }
    
    // Улучшенная реализация нормального распределения с использованием Box-Muller transform
    inline double normal_dist(double mean, double stddev) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::normal_distribution<double> dist(0, 1);
        
        return mean + stddev * dist(gen);
    }
}

#endif