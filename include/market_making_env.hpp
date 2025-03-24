#ifndef MARKET_MAKING_ENV_HPP
#define MARKET_MAKING_ENV_HPP

#include "market_maker.hpp"
#include <vector>
#include <array>
#include <random>

class MarketMakingEnv {
public:
    MarketMakingEnv(MarketMaker& mm);
    
    // Reset environment to initial state
    std::vector<double> reset();
    
    // Execute one step in the environment
    std::tuple<std::vector<double>, double, bool> step(const std::array<double, 4>& action);
    
    // Get current state
    std::vector<double> get_state() const;
    
private:
    MarketMaker& mm_;
    double current_inventory_;
    double current_profit_;
    int current_step_;
    int max_steps_;
    std::default_random_engine rng_;
    
    // State components
    double mid_price_;
    double sigma_;
    double latency_;
    double pool_depth_;
};

#endif