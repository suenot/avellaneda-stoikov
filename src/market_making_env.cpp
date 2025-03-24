#include "market_making_env.hpp"
#include <cmath>
#include <algorithm>

MarketMakingEnv::MarketMakingEnv(MarketMaker& mm) 
    : mm_(mm), current_inventory_(0), current_profit_(0), 
      current_step_(0), max_steps_(1000), rng_(std::random_device{}()) {}

std::vector<double> MarketMakingEnv::reset() {
    current_inventory_ = 0;
    current_profit_ = 0;
    current_step_ = 0;
    
    // Get initial market data
    auto [mid_price, bid, ask, bid_vol, ask_vol] = mm_.get_binance_data("USD+/wETH");
    auto [gas_price, latency] = mm_.get_onchain_metrics();
    
    // Initialize state
    mid_price_ = mid_price;
    sigma_ = mm_.calculate_volatility({mid_price}, 1); // Initial volatility
    latency_ = latency;
    pool_depth_ = 1000.0; // Placeholder for PMM pool depth
    
    return get_state();
}

std::tuple<std::vector<double>, double, bool> MarketMakingEnv::step(const std::array<double, 4>& action) {
    // Unpack action: [delta_a, delta_b, volume_a, volume_b]
    double delta_a = action[0];
    double delta_b = action[1];
    double volume_a = action[2];
    double volume_b = action[3];
    
    // Execute market making with current spreads
    mm_.step(mid_price_, sigma_, latency_, 0, volume_a);
    
    // Update state
    current_step_++;
    bool done = (current_step_ >= max_steps_);
    
    // Track executed trades and update profit
    auto [mid_price, bid, ask, bid_vol, ask_vol] = mm_.get_binance_data("USD+/wETH");
    auto [gas_price, latency] = mm_.get_onchain_metrics();
    
    // Calculate profit from last step (simplified)
    double spread_profit = (ask - bid) * 0.1; // 10% of spread as profit estimate
    current_profit_ += spread_profit;
    
    // Calculate reward components
    double profit_term = current_profit_;
    double inventory_risk = std::abs(current_inventory_) * sigma_ * 0.5; // Risk proportional to volatility
    double gas_cost = mm_.calculate_gas_cost(gas_price, volume_a + volume_b);
    
    // Final reward
    double reward = profit_term - inventory_risk - gas_cost;
    
    return {get_state(), reward, done};
}

std::vector<double> MarketMakingEnv::get_state() const {
    return {
        mid_price_,
        current_inventory_,
        sigma_,
        latency_,
        pool_depth_
    };
}