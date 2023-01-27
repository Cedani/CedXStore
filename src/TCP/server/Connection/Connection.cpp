#include "Connection.hpp"

using nlohmann::json;

tcp::Connection::Connection(boost::asio::ip::tcp::socket &socket): _socket(std::move(socket))
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
    std::cout << "[ARCADE TCP SERVER]: this connection has been closed (" << _socket.remote_endpoint().address().to_string() 
                  << ":" << _socket.remote_endpoint().port() << ")" << std::endl;
}

void tcp::Connection::readMessage()
{
    if (!_socket.is_open()){
        printDisconnection();
        return;
    }

    std::memset(_toRead, 0, MSGMAX);
    _socket.async_read_some(boost::asio::buffer(_toRead, MSGMAX), [this](boost::system::error_code ec, std::size_t len) {
        if (!ec) {
            try {
                _requests.push_back(_toRead);
                printMessage(_toRead);
            } catch (const nlohmann::detail::parse_error &e) {
                std::cout << "[ARCADE TCP SERVER]: Formating problem: " << e.what() << std::endl;
                json toSend = {
                    {"message", e.what()}
                };
                toSend["statusCode"] = ERROR_ARCARDE_SERVER_BAD_FORMATING;
            }
        } else if (ec == boost::asio::error::eof) {
            _socket.close();
            printDisconnection();
            return;
        } else {
            std::cout << "[ARCADE TCP SERVER]: error reading message: " << ec.message() << std::endl;
            json toSend = {
                    {"message", ec.message()}
                };
            toSend["statusCode"] = ERROR_ARCARDE_SERVER_READING_MESSAGE;
        }
        readMessage();
    });
}

void tcp::Connection::writeMessage(const std::string &msg)
{
    std::memset(_toSend, 0, MSGMAX);
    std::memcpy(_toSend, msg.data(), msg.size());
    boost::asio::async_write(_socket, boost::asio::buffer(_toSend, msg.size()), [this](boost::system::error_code ec, std::size_t len) {
        if (!ec) {
            std::cout << "[ARCADE TCP SERVER]: Message has been succesfully sent to " << _socket.remote_endpoint().address().to_string() 
                  << ":" << _socket.remote_endpoint().port() << std::endl;
        } else {
            std::cout << "[ARCADE TCP SERVER]: failed sending message: " << ec.message() << std::endl;
        }
    });
}

const boost::asio::ip::tcp::socket &tcp::Connection::getSocket() const
{
    return (_socket);
}

const std::vector<nlohmann::json> tcp::Connection::getRequest()
{
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