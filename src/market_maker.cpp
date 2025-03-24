#include "market_maker.hpp"
#include "utils.hpp"

MarketMaker::MarketMaker(double gamma, double T)
    : gamma_(gamma), T_(T), inventory_(0.0) {}

std::pair<double, double> MarketMaker::calculate_spreads(double S_t, double sigma, double k, double q_t) {
    // Calculate the reservation spread (half the bid-ask spread)
    double term1 = (1.0 / gamma_) * utils::log(1.0 + gamma_ / k);
    
    // Increase inventory impact by adding a stronger mean-reversion component
    // This will push prices down when inventory is positive and up when negative
    double inventory_factor = 2.0;  // Reduced to avoid excessive spread movement
    double term2 = q_t * sigma * sigma * T_ * inventory_factor;
    
    // Add a minimum spread to ensure delta_a > delta_b
    double min_spread = 0.5;  // Increased minimum spread for more trading opportunities
    
    // Calculate reservation price with stronger mean-reversion
    double reservation_price = S_t + term2;
    
    // Widen spreads as inventory deviates from zero (in either direction)
    double inventory_risk = std::abs(q_t) * 0.1;  // Additional spread widening based on inventory size
    double half_spread = std::max(term1 + inventory_risk, min_spread/2);
    
    // Calculate base spreads
    double delta_a = reservation_price + half_spread;  // Ask price
    double delta_b = reservation_price - half_spread;  // Bid price
    
    // Ensure delta_a is always greater than delta_b
    if (delta_a <= delta_b) {
        std::cout << "Warning: Invalid spread calculation corrected. "
                  << "Original delta_a=" << delta_a << ", delta_b=" << delta_b << std::endl;
        
        // Force a minimum spread
        double mid = (delta_a + delta_b) / 2;
        delta_a = mid + min_spread/2;
        delta_b = mid - min_spread/2;
    }
    
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
    // Get current inventory
    double current_inventory = inventory_.get_inventory();
    
    // Calculate spreads based on current market conditions and inventory
    auto [delta_a, delta_b] = calculate_spreads(S_t, sigma, k, current_inventory);
    auto [adjusted_delta_a, adjusted_delta_b] = adjust_spreads_for_onchain(S_t, delta_a, delta_b, latency, sigma, gas_cost);

    // Generate a random market price with balanced volatility
    double trade_size = 1.0;
    double volatility_factor = 1.0;  // Reduced volatility for more balanced trading
    
    // Calculate the reservation price (mid price adjusted for inventory)
    double reservation_price = S_t + current_inventory * sigma * sigma * T_ * 5.0;
    
    // Generate market price around the reservation price
    // This creates a strong mean-reversion effect to balance inventory
    double market_price = reservation_price + utils::normal_dist(0.0, sigma * volatility_factor);
    
    // Determine if trades should occur based on market price and spreads
    bool is_buy = (market_price <= delta_b);
    bool is_sell = (market_price >= delta_a);
    
    // This should not happen with our improved spread calculation, but just in case
    if (is_buy && is_sell) {
        std::cout << "Warning: Both buy and sell conditions triggered! "
                  << "Market price: " << market_price
                  << ", Ask: " << delta_a
                  << ", Bid: " << delta_b << std::endl;
        
        // Resolve the conflict based on inventory position
        if (current_inventory > 0) {
            is_buy = false;  // Prioritize selling to reduce positive inventory
        } else if (current_inventory < 0) {
            is_sell = false; // Prioritize buying to reduce negative inventory
        } else {
            // If inventory is exactly 0, randomly choose
            is_buy = (utils::normal_dist(0, 1) > 0);
            is_sell = !is_buy;
        }
    }
    
    // Execute trades and update inventory
    if (is_buy) {
        inventory_.update_inventory(trade_size, true);
        std::cout << "Trade: BUY at " << delta_b << " (Market price: " << market_price << ") ";
    } else if (is_sell) {
        inventory_.update_inventory(trade_size, false);
        std::cout << "Trade: SELL at " << delta_a << " (Market price: " << market_price << ") ";
    } else {
        std::cout << "No trade ";
    }

    // Output current state
    std::cout << "S_t: " << S_t
              << ", Inventory: " << inventory_.get_inventory()
              << ", Base Ask: " << delta_a << ", Base Bid: " << delta_b
              << ", Adjusted Ask: " << adjusted_delta_a << ", Adjusted Bid: " << adjusted_delta_b
              << ", Market Price: " << market_price
              << std::endl;
}