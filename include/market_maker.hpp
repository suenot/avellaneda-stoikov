#ifndef MARKET_MAKER_HPP
#define MARKET_MAKER_HPP

#include "inventory_manager.hpp"
#include <utility>
#include <vector>
#include <string>

class MarketMaker {
public:
    MarketMaker(double gamma = 0.1, double T = 300.0);

    // Вычисление спредов по модели Avellaneda-Stoikov
    std::pair<double, double> calculate_spreads(double S_t, double sigma, double k, double q_t);

    // Адаптация под onchain (с учетом latency и gas costs)
    std::pair<double, double> adjust_spreads_for_onchain(double S_t, double delta_a, double delta_b,
                                                        double latency, double sigma, double gas_cost,
                                                        double trade_size);

    // Симуляция шага маркет-мейкинга
    void step(double S_t, double sigma, double latency, double gas_cost, double trade_size);

    // Получение данных с Binance (заглушка до реализации API)
    std::tuple<double, double, double, double, double> get_binance_data(const std::string& pair);

    // Получение onchain-метрик (заглушка до реализации web3)
    std::pair<double, double> get_onchain_metrics();

    // Вычисление волатильности из исторических данных
    double calculate_volatility(const std::vector<double>& prices, int window = 5);

    // Расчет стоимости газа для сделки
    double calculate_gas_cost(double gas_price, double trade_size);

    // Корректировка цены с учетом задержки
    double adjust_price_with_latency(double S_t, double sigma, double latency);

    // Корректировка спредов под PMM-пулы
    std::pair<double, double> adjust_spreads_for_pmm(double S_t, double delta_a, double delta_b, double pool_depth);

    // Оценка интенсивности ордеров (k)
    double estimate_order_intensity(double bid, double ask, double bid_volume, double ask_volume);

private:
    double gamma_;  // Коэффициент риска
    double T_;      // Горизонт времени
    InventoryManager inventory_;
};

#endif