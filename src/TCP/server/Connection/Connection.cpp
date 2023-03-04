#include "Connection.hpp"

using nlohmann::json;

tcp::Connection::Connection(asio::ip::tcp::socket &socket): _socket(std::move(socket)), _isConnected(true)
{

}

tcp::Connection::~Connection()
{
    if (_socket.is_open())
        _socket.close();
}

void tcp::Connection::printMessage(const std::string &msg)
{
    std::cout << "[ARCADE TCP SERVER: New message from " << _socket.remote_endpoint().address().to_string() 
                  << ":" << _socket.remote_endpoint().port() << std::endl;
    std::cout << ":\n\t" << msg << std::endl;
}

void tcp::Connection::printDisconnection()
{
    // std::cout << "[ARCADE TCP SERVER]: this connection has been closed (" << _socket.remote_endpoint().address().to_string() 
    //               << ":" << _socket.remote_endpoint().port() << ")" << std::endl;
    std::cout << "[ARCADE TCP SERVER]: a connection has been closed" << std::endl;
}

void tcp::Connection::readMessage()
{
    if (!_socket.is_open()){
        // printDisconnection();
        return;
    }

    std::memset(_toRead, 0, MSGMAX);
    _socket.async_read_some(asio::buffer(_toRead, MSGMAX), [this](asio::error_code ec, std::size_t len) {
        if (!ec) {
            try {
                {
                    std::unique_lock<std::shared_mutex> _lock(_mutex);
                    _requests.push_back(json::parse(_toRead));
                }
                printMessage(_toRead);
            } catch (const nlohmann::detail::parse_error &e) {
                std::cout << "[ARCADE TCP SERVER]: Formating problem: " << e.what() << std::endl;
                json toSend = {
                    {"message", e.what()},
                    {"statusCode", ERROR_ARCARDE_SERVER_BAD_FORMATING}
                };
                // toSend["statusCode"] = ERROR_ARCARDE_SERVER_BAD_FORMATING;
                writeMessage(toSend.dump());
            }
        } else if (ec == asio::error::eof) {
            printDisconnection();
            _isConnected.store(false);
            // _socket.close();
            return;
            // _socket.cancel()l
            // if (_socket.is_open())
            //     std::cout << "socket has been closed" << std::endl;
        } else {
            std::cout << "[ARCADE TCP SERVER]: error reading message: " << ec.message() << std::endl;
            json toSend = {
                    {"message", ec.message()},
                    {"statusCode", ERROR_ARCARDE_SERVER_READING_MESSAGE}
                };
            // toSend["statusCode"] = ERROR_ARCARDE_SERVER_READING_MESSAGE;
            writeMessage(toSend);
            return;
        }
        readMessage();
    });
}

void tcp::Connection::writeMessage(const std::string &msg)
{
    std::memset(_toSend, 0, MSGMAX);
    std::memcpy(_toSend, msg.data(), msg.size());
    asio::async_write(_socket, asio::buffer(_toSend, msg.size()), [this](asio::error_code ec, std::size_t len) {
        if (!ec) {
            std::cout << "[ARCADE TCP SERVER]: Message has been succesfully sent to " << _socket.remote_endpoint().address().to_string() 
                  << ":" << _socket.remote_endpoint().port() << std::endl;
        } else {
            std::cout << "[ARCADE TCP SERVER]: failed sending message: " << ec.message() << std::endl;
        }
    });
}

const asio::ip::tcp::socket &tcp::Connection::getSocket() const
{
    return (_socket);
}


std::size_t tcp::Connection::getRequestSize() const
{
    std::shared_lock<std::shared_mutex>(_mutex);
    return (_requests.size());
}

const std::vector<nlohmann::json> tcp::Connection::getRequest()
{
    std::unique_lock<std::shared_mutex> lock(_mutex);
    std::vector<nlohmann::json> tmp = _requests;
    _requests.clear();
    return (tmp);
}

bool tcp::Connection::operator==(const Connection &con)
{
    if (_socket.remote_endpoint().address().to_string() == con._socket.remote_endpoint().address().to_string()
        && _socket.remote_endpoint().port() == con._socket.remote_endpoint().port())
        return true;
    return false;
}

bool tcp::Connection::isSocketOpen() const
{
    std::shared_lock<std::shared_mutex> lock(_mutex);
    return (_isConnected.load());
}