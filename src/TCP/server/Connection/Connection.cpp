#include "Connection.hpp"

using nlohmann::json;

tcp::Connection::Connection(boost::asio::ip::tcp::socket &socket, const std::function<void(const nlohmann::json &, boost::shared_ptr<Connection>)> &func): _socket(std::move(socket)), _isConnected(true), addRequest(func)
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
        std::cout << "disconnected" << std::endl;
        // printDisconnection();
        return;
    }

    std::memset(_toRead, 0, MSGMAXR);
    _socket.async_read_some(boost::asio::buffer(_toRead, MSGMAXR), [this](boost::system::error_code ec, std::size_t len) {
        if (!ec) {
            try {
                {
                    std::unique_lock<std::shared_mutex> _lock(_mutex);
                    parseRead();
                    if (_messageEnd) {
                        // _requests.push_back(json::parse(_finishedBuffer));
                        _messageEnd = false;
                        addRequest(json::parse(_finishedBuffer), this->shared_from_this());
                        _finishedBuffer.clear();
                    }
                }
                printMessage(_toRead);
            } catch (const nlohmann::detail::parse_error &e) {
                std::cout << "[ARCADE TCP SERVER]: Formating problem: " << e.what() << std::endl;
                json toSend = {
                    {"message", e.what()},
                    {"statusCode", ERROR_ARCARDE_SERVER_BAD_FORMATING}
                };
                writeMessage(toSend.dump());
            }
        } else if (ec == boost::asio::error::eof) {
            printDisconnection();
            _isConnected.store(false);
            return;
        } else {
            std::cout << "[ARCADE TCP SERVER]: error reading message: " << ec.message() << std::endl;
            _socket.close();
            return;
        }
        readMessage();
    });
}

void tcp::Connection::writeMessage(const std::string &msg)
{
    std::memset(_toSend, 0, MSGMAXW);
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

void tcp::Connection::parseRead()
{
    std::string msg(_toRead);
    std::size_t idx = msg.find("\r\n");

    if (idx != msg.npos) {
        if ( idx + 2 == msg.size()) {
            _finishedBuffer = _readBuffer + msg;
            _readBuffer.clear();
            _messageEnd = true;
        } else {
            _finishedBuffer = _readBuffer + msg.substr(0, idx); 
            _readBuffer = _readBuffer.substr(idx + 2);
        }
    } else {
            _readBuffer += msg;
    }
}