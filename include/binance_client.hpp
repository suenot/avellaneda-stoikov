#ifndef BINANCE_CLIENT_HPP
#define BINANCE_CLIENT_HPP

#include <string>
#include <functional>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/strand.hpp>
#include <nlohmann/json.hpp>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = net::ip::tcp;
using json = nlohmann::json;

class BinanceClient {
public:
    using MarketDataCallback = std::function<void(const json&)>;
    
    BinanceClient(net::io_context& ioc);
    
    // Подключение к WebSocket и подписка на данные
    void connect_and_subscribe(const std::string& symbol, MarketDataCallback callback);
    
    // Структура для хранения рыночных данных
    struct MarketData {
        double bid_price;
        double bid_qty;
        double ask_price;
        double ask_qty;
        double last_price;
        uint64_t event_time;
    };

private:
    net::io_context& ioc_;
    websocket::stream<beast::tcp_stream> ws_;
    beast::flat_buffer buffer_;
    std::string host_ = "stream.binance.com";
    std::string port_ = "9443";
    MarketDataCallback callback_;
    
    void on_read(beast::error_code ec, std::size_t bytes_transferred);
    void run(const std::string& host, const std::string& port);
};

#endif // BINANCE_CLIENT_HPP