#include "market_maker.hpp"
#include "utils.hpp"
#include <iostream>
#include <cmath>

MarketMaker::MarketMaker(double gamma, double T)
    : gamma_(gamma), T_(T), inventory_(0.0) {}

std::pair<double, double> MarketMaker::calculate_spreads(double S_t, double sigma, double k, double q_t) {
    // Классическая формула Avellaneda-Stoikov с ограничениями
    double spread_term = (1.0 / gamma_) * utils::log(1.0 + gamma_ / k);
    
    // Ограничиваем влияние инвентаря и волатильности
    const double max_inventory_impact = 1.5;
    const double max_sigma = 0.3;
    double bounded_q = std::clamp(q_t, -max_inventory_impact, max_inventory_impact);
    double bounded_sigma = std::clamp(sigma, 0.01, max_sigma);
    double inventory_term = bounded_q * bounded_sigma * bounded_sigma * (T_/2);  // Ослабляем влияние времени
    
    // Дополнительная проверка на разумность спреда
    if (sigma > 0.2) {
        spread_term *= 1.2;  // Увеличиваем базовый спред при высокой волатильности
    }

    double delta_a = S_t + spread_term + inventory_term;  // Ask price
    double delta_b = S_t - spread_term - inventory_term;  // Bid price

    // Проверка на корректность спредов (delta_a должен быть больше delta_b)
    if (delta_a <= delta_b) {
        std::cout << "Warning: Invalid spread calculation detected. "
                  << "Original delta_a=" << delta_a << ", delta_b=" << delta_b << std::endl;
        
        // Обеспечиваем минимальный спред
        double mid = (delta_a + delta_b) / 2;
        double min_spread = 0.1;  // Минимальный спред для корректности
        delta_a = mid + min_spread/2;
        delta_b = mid - min_spread/2;
    }

    return {delta_a, delta_b};
}

std::pair<double, double> MarketMaker::adjust_spreads_for_onchain(double S_t, double delta_a, double delta_b,
                                                                 double latency, double sigma, double gas_cost,
                                                                 double trade_size) {
    // Корректировка цены с учетом latency (задача 3.2)
    double latency_adjustment = utils::normal_dist(0.0, sigma * std::sqrt(latency));
    double adjusted_S_t = S_t + latency_adjustment;

    // Gas cost пропорционален объему сделки (100,000 gas за ордер)
    double gas_penalty = (gas_cost * 100000 * trade_size) / 1e18; // В ETH, затем в единицы цены

    // Применяем latency_adjustment к обоим спредам
    double adjusted_delta_a = delta_a + latency_adjustment;
    double adjusted_delta_b = delta_b + latency_adjustment;

    return {adjusted_delta_a, adjusted_delta_b};
}

void MarketMaker::step(double S_t, double sigma, double k, double latency, double gas_cost, double trade_size) {
    // Получаем текущий инвентарь
    double current_inventory = inventory_.get_inventory();
    
    // Рассчитываем спреды на основе текущих рыночных условий и инвентаря
    auto [delta_a, delta_b] = calculate_spreads(S_t, sigma, k, current_inventory);
    auto [adjusted_delta_a, adjusted_delta_b] = adjust_spreads_for_onchain(S_t, delta_a, delta_b, latency, sigma, gas_cost, trade_size);

    // Генерация независимой рыночной цены (не зависит от reservation_price)
    double market_price = S_t + utils::normal_dist(0.0, sigma);
    
    // Учет gas_cost при исполнении сделки
    double gas_penalty = (gas_cost * 100000 * trade_size) / 1e18;

    // Определяем, должны ли произойти сделки на основе рыночной цены и спредов
    bool is_buy = (market_price <= adjusted_delta_b);
    bool is_sell = (market_price >= adjusted_delta_a);
    
    // Это не должно происходить с нашим улучшенным расчетом спреда, но на всякий случай
    if (is_buy && is_sell) {
        std::cout << "Warning: Both buy and sell conditions triggered! "
                  << "Market price: " << market_price
                  << ", Ask: " << adjusted_delta_a
                  << ", Bid: " << adjusted_delta_b << std::endl;
        
        // Разрешаем конфликт на основе позиции инвентаря
        if (current_inventory > 0) {
            is_buy = false;  // Приоритет продажи для уменьшения положительного инвентаря
        } else if (current_inventory < 0) {
            is_sell = false; // Приоритет покупки для уменьшения отрицательного инвентаря
        } else {
            // Если инвентарь равен 0, случайно выбираем
            is_buy = (utils::normal_dist(0, 1) > 0);
            is_sell = !is_buy;
        }
    }
    
    // Выполняем сделки и обновляем инвентарь
    if (is_buy) {
        inventory_.update_inventory(trade_size, true);
        std::cout << "Trade: BUY at " << adjusted_delta_b << " (Market price: " << market_price
                  << ", Gas cost: " << gas_penalty << ") ";
    } else if (is_sell) {
        inventory_.update_inventory(trade_size, false);
        std::cout << "Trade: SELL at " << adjusted_delta_a << " (Market price: " << market_price
                  << ", Gas cost: " << gas_penalty << ") ";
    } else {
        std::cout << "No trade ";
    }

    // Выводим текущее состояние
    std::cout << "S_t: " << S_t
              << ", Inventory: " << inventory_.get_inventory()
              << ", Base Ask: " << delta_a << ", Base Bid: " << delta_b
              << ", Adjusted Ask: " << adjusted_delta_a << ", Adjusted Bid: " << adjusted_delta_b
              << ", Market Price: " << market_price
              << std::endl;
}

std::pair<double, std::pair<double, double>> MarketMaker::get_binance_data(const std::string& pair) {
    // Заглушка: mid_price, bid, ask
    // TODO: Подключить Binance API через libcurl или Boost
    double mid_price = 2000.0;
    double bid = mid_price - 1.0;
    double ask = mid_price + 1.0;
    return {mid_price, {bid, ask}};
}

std::pair<double, double> MarketMaker::get_onchain_metrics() {
    // Заглушка: gas_price (wei), latency (seconds)
    // TODO: Подключить web3cpp для Infura/Alchemy
    return {50e9, 12.0};
}

double MarketMaker::calculate_volatility(const std::vector<double>& prices, int window) {
    if (prices.size() < window) return 0.0;

    std::vector<double> returns;
    for (size_t i = 1; i < prices.size(); ++i) {
        returns.push_back(std::log(prices[i] / prices[i - 1]));
    }

    double mean = 0.0;
    for (double r : returns) mean += r;
    mean /= returns.size();

    double variance = 0.0;
    for (double r : returns) variance += (r - mean) * (r - mean);
    variance /= (returns.size() - 1);

    return std::sqrt(variance) * std::sqrt(60); // Масштабируем до минутной волатильности
}