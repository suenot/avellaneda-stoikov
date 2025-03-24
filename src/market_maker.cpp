#include "market_maker.hpp"
#include "utils.hpp"

MarketMaker::MarketMaker(double gamma, double T)
    : gamma_(gamma), T_(T), inventory_(0.0) {}

std::pair<double, double> MarketMaker::calculate_spreads(double S_t, double sigma, double k, double q_t) {
    double term1 = (1.0 / gamma_) * utils::log(1.0 + gamma_ / k);
    double term2 = q_t * sigma * sigma * T_;

    double delta_a = S_t + term1 + term2;  // Спред продажи
    double delta_b = S_t - term1 - term2;  // Спред покупки

    return {delta_a, delta_b};
}

std::pair<double, double> MarketMaker::adjust_spreads_for_onchain(double S_t, double delta_a, double delta_b,
                                                                 double latency, double sigma, double gas_cost) {
    double latency_adjustment = utils::normal_dist(0.0, sigma * std::sqrt(latency));
    double adjusted_S_t = S_t + latency_adjustment;

    double gas_penalty = (gas_cost / 1e18) * 10000;  // 0.0005 ETH
    double spread_adjustment = gas_penalty * 10;     // ~0.5 единиц

    double adjusted_delta_a = adjusted_S_t + (delta_a - S_t) + spread_adjustment;
    double adjusted_delta_b = adjusted_S_t - (S_t - delta_b) - spread_adjustment;

    return {adjusted_delta_a, adjusted_delta_b};
}

void MarketMaker::step(double S_t, double sigma, double k, double latency, double gas_cost) {
    auto [delta_a, delta_b] = calculate_spreads(S_t, sigma, k, inventory_.get_inventory());
    auto [adjusted_delta_a, adjusted_delta_b] = adjust_spreads_for_onchain(S_t, delta_a, delta_b, latency, sigma, gas_cost);

    double trade_size = 1.0;
    double market_price = S_t + utils::normal_dist(0.0, sigma);
    
    // More aggressive trading conditions
    bool is_buy = (market_price <= adjusted_delta_b);  // Changed from delta_b
    bool is_sell = (market_price >= adjusted_delta_a); // Changed from delta_a
    
    if (is_buy) {
        inventory_.update_inventory(trade_size, true);
    } else if (is_sell) {
        inventory_.update_inventory(trade_size, false);
    }

    std::cout << "S_t: " << S_t
              << ", Inventory: " << inventory_.get_inventory()
              << ", Base Ask: " << delta_a << ", Base Bid: " << delta_b
              << ", Adjusted Ask: " << adjusted_delta_a << ", Adjusted Bid: " << adjusted_delta_b
              << ", Market Price: " << market_price  // Добавляем для отладки
              << std::endl;
}