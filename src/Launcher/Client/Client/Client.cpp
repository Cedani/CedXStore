#include "Client.hpp"

using nlohmann::json;

lau::Client::Client(): _sock(_ctx)
{
    std::cout << "starting client" << std::endl;
}

bool lau::Client::connectToServer()
{
    std::ifstream endPointFile(TEMP_ENDPOINT);

    if (!endPointFile.is_open())
        return;
    std::string port;
    std::string hostName;
    // int port;
    std::getline(endPointFile, hostName);
    std::getline(endPointFile, port);
    boost::asio::ip::tcp::resolver resolver(_ctx);
    boost::asio::ip::tcp::resolver::results_type endpoints= resolver.resolve(hostName, port);
    boost::asio::connect(_sock, endpoints);
    return true;
}

json lau::Client::readMessage()
{
    std::memset(_read, 0, MSGMAXR);
    boost::system::error_code ec;
    _sock.read_some(boost::asio::buffer(_read, MSGMAXR), ec);
    if (!ec) {
        try {
            return json::parse(_read);
        } catch (const nlohmann::detail::parse_error &e) {
            std::cout << "[ARCADE TCP CLIENT]: Formating problem: " << e.what() << std::endl;
        }
    } else if (ec == boost::asio::error::eof) {
        std::cout << "[ARCADE TCP CLIENT]: Server disconnected or has ended connection" << std::endl;
        return json{};
    }
    std::cout << "[ARCADE TCP CLIENT]: error reading message: " << ec.message() << std::endl;
    return json{};
}

bool lau::Client::writeMessage(const std::string &msg)
{
    std::memset(_write, 0, MSGMAXW);
    std::memcpy(_write, msg.data(), msg.size());
    boost::system::error_code ec;
    _sock.write_some(_write, ec);
    if (!ec) {
        std::cout << "[ARCADE TCP CLIENT]: Message has been succesfully sent to Server" << std::endl;
        return true;
    }
    std::cout << "[ARCADE TCP SERVER]: failed sending message: " << ec.message() << std::endl;
    return false;
}