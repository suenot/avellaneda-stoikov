#include "market_maker.hpp"
#include <iostream>
#include <fstream>

class MarketSimulator {
public:
    MarketSimulator(MarketMaker& mm) : mm_(mm) {}

    void run_simulation(int steps) {
        std::ofstream log_file("simulation_log.csv");
        log_file << "step,price,spread\n";

        for (int i = 0; i < steps; ++i) {
            // Get market data using tuple (no structured binding)
            auto data = mm_.get_binance_data("USD+/wETH");
            double mid_price = std::get<0>(data);
            double bid = std::get<1>(data);
            double ask = std::get<2>(data);
            double spread = ask - bid;
            
            // Log basic market data
            log_file << i << ","
                    << mid_price << ","
                    << spread << "\n";
        }
    }

private:
    MarketMaker& mm_;
};

int main() {
    MarketMaker mm;
    MarketSimulator simulator(mm);
    simulator.run_simulation(1000);
    return 0;
}