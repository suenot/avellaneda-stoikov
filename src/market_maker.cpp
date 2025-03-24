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
    // Корректировка цены с учетом задержки (latency)
    double latency_adjustment = utils::normal_dist(0.0, sigma * std::sqrt(latency));
    double adjusted_S_t = S_t + latency_adjustment;

    // Gas cost: переводим в ETH и учитываем как заметную корректировку спреда
    double gas_penalty = (gas_cost / 1e18) * 100000;  // 50 Gwei * 100,000 gas = 0.005 ETH
    double spread_adjustment = gas_penalty * 100;     // Увеличиваем влияние до ~0.5 единиц

    double adjusted_delta_a = adjusted_S_t + (delta_a - S_t) + spread_adjustment;
    double adjusted_delta_b = adjusted_S_t - (S_t - delta_b) - spread_adjustment;

    return {adjusted_delta_a, adjusted_delta_b};
}

void MarketMaker::step(double S_t, double sigma, double k, double latency, double gas_cost) {
    // 1. Вычисляем базовые спреды A-S
    auto [delta_a, delta_b] = calculate_spreads(S_t, sigma, k, inventory_.get_inventory());

    // 2. Адаптируем под onchain
    auto [adjusted_delta_a, adjusted_delta_b] = adjust_spreads_for_onchain(S_t, delta_a, delta_b, latency, sigma, gas_cost);

    // 3. Симулируем сделку с учетом спредов
    double trade_size = 1.0;
    double market_price = S_t + utils::normal_dist(0.0, sigma);  // Симулируем рыночную цену
    bool is_buy = (market_price <= delta_b);  // Покупка, если цена ниже bid
    bool is_sell = (market_price >= delta_a); // Продажа, если цена выше ask
    if (is_buy) {
        inventory_.update_inventory(trade_size, true);
    } else if (is_sell) {
        inventory_.update_inventory(trade_size, false);
    }

    // 4. Выводим результат
    std::cout << "S_t: " << S_t
              << ", Inventory: " << inventory_.get_inventory()
              << ", Base Ask: " << delta_a << ", Base Bid: " << delta_b
              << ", Adjusted Ask: " << adjusted_delta_a << ", Adjusted Bid: " << adjusted_delta_b
              << std::endl;
}