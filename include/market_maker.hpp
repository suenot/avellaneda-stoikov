#ifndef MARKET_MAKER_HPP
#define MARKET_MAKER_HPP

#include "inventory_manager.hpp"
#include <utility>

class MarketMaker {
public:
    MarketMaker(double gamma = 0.1, double T = 300.0);

    // Вычисление спредов по модели Avellaneda-Stoikov
    std::pair<double, double> calculate_spreads(double S_t, double sigma, double k, double q_t);

    // Адаптация под onchain (с учетом latency и gas costs)
    std::pair<double, double> adjust_spreads_for_onchain(double S_t, double delta_a, double delta_b,
                                                        double latency, double sigma, double gas_cost);

    // Симуляция шага маркет-мейкинга
    void step(double S_t, double sigma, double k, double latency, double gas_cost);

private:
    double gamma_;  // Коэффициент риска
    double T_;      // Горизонт времени
    InventoryManager inventory_;
};

#endif