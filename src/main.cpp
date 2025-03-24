#include "market_maker.hpp"
#include "utils.hpp"
#include <iostream>

int main() {
    MarketMaker mm(0.1, 300.0);  // gamma = 0.1, T = 300 секунд

    // Примерные данные (заглушки вместо API)
    double S_t = 2000.0;  // Mid-price (USD+/wETH)
    double sigma = 0.02;  // Волатильность (2%)
    double k = 0.5;       // Интенсивность ордеров
    double latency = 12.0; // Задержка (12 секунд)
    double gas_cost = 50e9; // Gas price в wei (50 Gwei)

    // Инициализация генератора случайных чисел
    std::srand(static_cast<unsigned>(time(nullptr)));

    // Симуляция 10 шагов
    for (int i = 0; i < 10; ++i) {
        std::cout << "Step " << i + 1 << ": ";
        mm.step(S_t, sigma, k, latency, gas_cost);

        // Симулируем небольшое изменение цены
        S_t += utils::normal_dist(0.0, sigma);
    }

    return 0;
}