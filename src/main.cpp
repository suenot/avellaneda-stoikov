#include "market_maker.hpp"
#include "utils.hpp"
#include <iostream>

int main() {
    MarketMaker mm(0.1, 30.0);  // gamma = 0.1, T = 30 seconds

    double S_t = 2000.0;
    double sigma = 0.2;   // Increased volatility to 20%
    double k = 10.0;      // Increased order intensity
    double latency = 12.0;
    double gas_cost = 50e9;

    for (int i = 0; i < 10; ++i) {
        std::cout << "Step " << i + 1 << ": ";
        mm.step(S_t, sigma, k, latency, gas_cost);
        S_t += utils::normal_dist(0.0, sigma);
    }

    return 0;
}