#include "market_maker.hpp"
#include "utils.hpp"

MarketMaker::MarketMaker(double gamma, double T)
    : gamma_(gamma), T_(T), inventory_(0.0) {}

std::pair<double, double> MarketMaker::calculate_spreads(double S_t, double sigma, double k, double q_t) {
    double term1 = (1.0 / gamma_) * utils::log(1.0 + gamma_ / k);
    double term2 = q_t * sigma * sigma * T_;

    // Ensure delta_a is always greater than delta_b (no zero spread)
    double delta_a = S_t + term1 + term2;  // Спред продажи
    double delta_b = S_t - term1 - term2;  // Спред покупки

    // Debug output to verify calculations
    // std::cout << "Debug: term1=" << term1 << ", term2=" << term2 << ", q_t=" << q_t << std::endl;

    return {delta_a, delta_b};
}

std::pair<double, double> MarketMaker::adjust_spreads_for_onchain(double S_t, double delta_a, double delta_b,
                                                                 double latency, double sigma, double gas_cost) {
    // Reduce the impact of latency_adjustment by 50%
    double latency_adjustment = utils::normal_dist(0.0, sigma * std::sqrt(latency)) * 0.5;
    double adjusted_S_t = S_t + latency_adjustment;

    double gas_penalty = (gas_cost / 1e18) * 100000;  // 0.005 ETH
    double spread_adjustment = gas_penalty * 100;     // ~0.5 единиц

    double adjusted_delta_a = delta_a + spread_adjustment + latency_adjustment;
    double adjusted_delta_b = delta_b - spread_adjustment + latency_adjustment;

    return {adjusted_delta_a, adjusted_delta_b};
}

void MarketMaker::step(double S_t, double sigma, double k, double latency, double gas_cost) {
    auto [delta_a, delta_b] = calculate_spreads(S_t, sigma, k, inventory_.get_inventory());
    auto [adjusted_delta_a, adjusted_delta_b] = adjust_spreads_for_onchain(S_t, delta_a, delta_b, latency, sigma, gas_cost);

    double trade_size = 1.0;
    double market_price = S_t + utils::normal_dist(0.0, sigma);
    
    // Use base spreads for trading conditions to increase trade frequency
    bool is_buy = (market_price <= delta_b);  // Changed to base spread
    bool is_sell = (market_price >= delta_a); // Changed to base spread
    
    if (is_buy) {
        inventory_.update_inventory(trade_size, true);
    } else if (is_sell) {
        inventory_.update_inventory(trade_size, false);
    }

    std::cout << "S_t: " << S_t
              << ", Inventory: " << inventory_.get_inventory()
              << ", Base Ask: " << delta_a << ", Base Bid: " << delta_b
              << ", Adjusted Ask: " << adjusted_delta_a << ", Adjusted Bid: " << adjusted_delta_b
              << ", Market Price: " << market_price
              << std::endl;
}