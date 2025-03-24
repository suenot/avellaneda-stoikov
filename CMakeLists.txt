cmake_minimum_required(VERSION 3.10)
project(MarketMaker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# Находим Boost (Beast для WebSocket)
find_package(Boost 1.70.0 COMPONENTS system REQUIRED)
find_package(nlohmann_json 3.11.2 REQUIRED)

# Явно указываем пути включения Boost
include_directories(${Boost_INCLUDE_DIRS})

# Позже можно добавить web3cpp для onchain-данных
# find_package(web3cpp) # Раскомментировать, когда добавите web3cpp

add_executable(market_maker
    src/main.cpp
    src/market_maker.cpp
    src/inventory_manager.cpp
    src/binance_client.cpp
    src/market_making_env.cpp
)

# Находим PyTorch
find_package(Torch REQUIRED)

# Подключаем библиотеки
find_package(gym REQUIRED)
target_link_libraries(market_maker
    Boost::system
    ${nlohmann_json_LIBRARIES}
    gym::gym
    ${TORCH_LIBRARIES}
)

# Добавляем PPO trainer
add_executable(ppo_trainer
    src/ppo_trainer.cpp
    src/market_making_env.cpp
    src/market_maker.cpp
    src/inventory_manager.cpp
    src/binance_client.cpp
)
target_link_libraries(ppo_trainer
    ${TORCH_LIBRARIES}
    Boost::system
    ${nlohmann_json_LIBRARIES}
)
# target_link_libraries(market_maker web3cpp) # Раскомментировать позже