#include "Client.hpp"

using nlohmann::json;

lau::Client::Client(): _sock(_ctx)
{
    std::cout << "starting client" << std::endl;
}

bool lau::Client::connectToServer()
{
    try {
        std::ifstream endPointFile(TEMP_ENDPOINT);

        if (!endPointFile.is_open())
            return false;
        std::string port;
        std::string hostName;
        std::getline(endPointFile, hostName);
        std::getline(endPointFile, port);
        boost::asio::ip::tcp::resolver resolver(_ctx);
        boost::asio::ip::tcp::resolver::results_type endpoints= resolver.resolve(hostName, port);
        boost::asio::connect(_sock, endpoints);
        return true;
    } catch (const boost::system::system_error& e) {
        std::cout << "exception connect: " << e.code() << " " << e.what() << std::endl;
        return false;
    }
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
    _sock.write_some(boost::asio::buffer(_write, msg.size()), ec);
    if (!ec) {
        std::cout << "[ARCADE TCP CLIENT]: Message has been succesfully sent to Server" << std::endl;
        return true;
    }
    std::cout << "[ARCADE TCP SERVER]: failed sending message: " << ec.message() << std::endl;
    return false;
}