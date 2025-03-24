#ifndef INVENTORY_MANAGER_HPP
#define INVENTORY_MANAGER_HPP

class InventoryManager {
public:
    InventoryManager(double initial_inventory = 0.0) : inventory_(initial_inventory) {}

    void update_inventory(double trade_size, bool is_buy) {
        if (is_buy) {
            inventory_ += trade_size;
        } else {
            inventory_ -= trade_size;
        }
    }

    double get_inventory() const { return inventory_; }

private:
    double inventory_;
};

#endif