#include "market_making_env.hpp"
#include <torch/torch.h>
#include <torch/script.h>
#include <iostream>

class PPOTrainer {
public:
    PPOTrainer(MarketMakingEnv& env) 
        : env_(env),
          policy_net_(/* input_dim */ 5, /* hidden_dim */ 64, /* output_dim */ 4),
          value_net_(/* input_dim */ 5, /* hidden_dim */ 64, /* output_dim */ 1),
          optimizer_(policy_net_->parameters(), torch::optim::AdamOptions(1e-3)) {}

    void train(int episodes) {
        for (int ep = 0; ep < episodes; ++ep) {
            auto state = env_.reset();
            std::vector<torch::Tensor> states;
            std::vector<torch::Tensor> actions;
            std::vector<float> rewards;
            
            while (true) {
                // Get action from policy
                auto action_probs = policy_net_->forward(torch::tensor(state));
                auto action = action_probs.multinomial(1);
                
                // Step environment
                auto [next_state, reward, done] = env_.step(action);
                
                // Store transition
                states.push_back(torch::tensor(state));
                actions.push_back(action);
                rewards.push_back(reward);
                
                if (done) break;
                state = next_state;
            }
            
            // PPO update
            update_policy(states, actions, rewards);
            
            if (ep % 10 == 0) {
                std::cout << "Episode " << ep 
                          << ", Avg Reward: " << std::accumulate(rewards.begin(), rewards.end(), 0.0) / rewards.size()
                          << std::endl;
            }
        }
    }

private:
    void update_policy(const std::vector<torch::Tensor>& states,
                      const std::vector<torch::Tensor>& actions,
                      const std::vector<float>& rewards) {
        // Convert to tensors
        auto states_tensor = torch::stack(states);
        auto actions_tensor = torch::stack(actions);
        auto rewards_tensor = torch::tensor(rewards);
        
        // Calculate advantages
        auto values = value_net_->forward(states_tensor);
        auto advantages = rewards_tensor - values;
        
        // PPO loss
        auto old_probs = policy_net_->forward(states_tensor).gather(1, actions_tensor);
        auto new_probs = policy_net_->forward(states_tensor).gather(1, actions_tensor);
        
        auto ratio = (new_probs / old_probs).exp();
        auto clip_ratio = torch::clamp(ratio, 1.0 - 0.2, 1.0 + 0.2);
        auto policy_loss = -torch::min(ratio * advantages, clip_ratio * advantages).mean();
        
        auto value_loss = torch::mse_loss(values, rewards_tensor);
        
        // Update
        optimizer_.zero_grad();
        (policy_loss + value_loss).backward();
        optimizer_.step();
    }

    MarketMakingEnv& env_;
    torch::nn::Sequential policy_net_;
    torch::nn::Sequential value_net_;
    torch::optim::Adam optimizer_;
};

int main() {
    MarketMaker mm;
    MarketMakingEnv env(mm);
    PPOTrainer trainer(env);
    
    trainer.train(1000);
    return 0;
}