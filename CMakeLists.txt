cmake_minimum_required(VERSION 3.10)
project(MarketMaker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# Находим Boost для работы с сетью (для Binance API)
find_package(Boost COMPONENTS system REQUIRED)
# Позже можно добавить web3cpp для onchain-данных
# find_package(web3cpp) # Раскомментировать, когда добавите web3cpp

add_executable(market_maker
    src/main.cpp
    src/market_maker.cpp
    src/inventory_manager.cpp
)

# Подключаем библиотеки
target_link_libraries(market_maker Boost::system)
# target_link_libraries(market_maker web3cpp) # Раскомментировать позже