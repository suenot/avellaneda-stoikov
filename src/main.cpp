#include "market_maker.hpp"
#include "utils.hpp"
#include <iostream>
#include <vector>

int main() {
    // Используем T = 300 секунд, как указано в задаче
    MarketMaker mm(0.1, 300.0);

    // Симуляция исторических данных для волатильности
    std::vector<double> prices = {2000.0};
    double S_t = 2000.0;
    double trade_size = 1.0;

    // Увеличиваем начальную волатильность для более активной торговли
    double initial_sigma = 0.05;  // 5% волатильность

    for (int i = 0; i < 300; ++i) {
        std::cout << "Step " << i + 1 << ": ";

        // Получение данных (заглушки)
        auto [mid_price, bid_ask] = mm.get_binance_data("USD+/wETH");
        auto [gas_cost, latency] = mm.get_onchain_metrics();
        
        // Добавляем случайное движение цены для симуляции реального рынка
        S_t = mid_price + utils::normal_dist(0.0, mid_price * 0.01);  // 1% случайное движение

        // Вычисление волатильности
        double sigma = mm.calculate_volatility(prices, 5);
        if (sigma < 0.01) sigma = initial_sigma;  // Минимальная волатильность для активной торговли

        // Выводим текущее значение sigma
        std::cout << "Sigma: " << sigma << ", ";

        // Интенсивность ордеров (заглушка, позже из книги ордеров)
        double k = 5.0;  // Уменьшаем k для более широких спредов

        mm.step(S_t, sigma, k, latency, gas_cost, trade_size);

        // Обновление цены для следующего шага с более выраженным случайным движением
        S_t += utils::normal_dist(0.0, S_t * 0.02);  // 2% случайное движение
        prices.push_back(S_t);
    }

    return 0;
}