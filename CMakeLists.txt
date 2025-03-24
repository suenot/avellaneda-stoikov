cmake_minimum_required(VERSION 3.10)
project(MarketMaker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(market_maker
    src/main.cpp
    src/market_maker.cpp
    src/inventory_manager.cpp
)

# Если добавите Boost или другие библиотеки, раскомментируйте:
# find_package(Boost REQUIRED)
# target_link_libraries(market_maker Boost::system)