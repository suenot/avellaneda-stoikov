#include "binance_client.hpp"
#include <iostream>
#include <thread>

BinanceClient::BinanceClient(net::io_context& ioc) 
    : ioc_(ioc), ws_(net::make_strand(ioc)) {}

void BinanceClient::connect_and_subscribe(const std::string& symbol, MarketDataCallback callback) {
    callback_ = callback;
    run(host_, port_);
    
    // Подписка на стакан ордеров и сделки
    std::string stream = symbol + "@depth@100ms";
    ws_.async_write(
        net::buffer(stream),
        [this](beast::error_code ec, std::size_t) {
            if(ec) {
                std::cerr << "Subscribe error: " << ec.message() << std::endl;
                return;
            }
            
            // Начинаем чтение данных
            ws_.async_read(
                buffer_,
                beast::bind_front_handler(
                    &BinanceClient::on_read,
                    this));
        });
}

void BinanceClient::run(const std::string& host, const std::string& port) {
    // Разрешаем доменное имя
    tcp::resolver resolver(ioc_);
    beast::get_lowest_layer(ws_).async_connect(
        resolver.resolve(host, port),
        [this](beast::error_code ec, tcp::resolver::results_type::endpoint_type) {
            if(ec) {
                std::cerr << "Connect error: " << ec.message() << std::endl;
                return;
            }
            
            // Устанавливаем WebSocket соединение
            ws_.async_handshake(
                host_, "/ws",
                [this](beast::error_code ec) {
                    if(ec) {
                        std::cerr << "Handshake error: " << ec.message() << std::endl;
                        return;
                    }
                });
        });
}

void BinanceClient::on_read(beast::error_code ec, std::size_t) {
    if(ec) {
        std::cerr << "Read error: " << ec.message() << std::endl;
        
        // Попытка переподключения через 5 секунд
        std::this_thread::sleep_for(std::chrono::seconds(5));
        run(host_, port_);
        return;
    }
    
    try {
        auto data = beast::buffers_to_string(buffer_.data());
        buffer_.consume(buffer_.size());
        
        json market_data = json::parse(data);
        
        // Обработка данных стакана
        if(market_data.contains("bids") && market_data.contains("asks")) {
            MarketData md;
            md.bid_price = std::stod(market_data["bids"][0][0].get<std::string>());
            md.bid_qty = std::stod(market_data["bids"][0][1].get<std::string>());
            md.ask_price = std::stod(market_data["asks"][0][0].get<std::string>());
            md.ask_qty = std::stod(market_data["asks"][0][1].get<std::string>());
            md.event_time = market_data["E"].get<uint64_t>();
            
            callback_(market_data);
        }
    } catch(const std::exception& e) {
        std::cerr << "Data processing error: " << e.what() << std::endl;
    }
    
    // Читаем следующее сообщение
    ws_.async_read(
        buffer_,
        beast::bind_front_handler(
            &BinanceClient::on_read,
            this));
}