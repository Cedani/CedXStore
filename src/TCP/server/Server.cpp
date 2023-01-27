#include "Server.hpp"

using nlohmann::json;

tcp::Server::Server(int port): _acceptor(_io, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), _threadPool(10)
{
    _threadPool.init();
}

tcp::Server::~Server()
{
    _acceptor.close();
    _io.stop();
    _threadContext.join();
}

bool tcp::Server::start()
{
    try {
        _threadContext = boost::thread([this](){_io.run();});
        waitConnections();
        std::cout << "[ARCADE TCP SERVER]: new server started (" << _acceptor.local_endpoint() << ")" << std::endl; 
    } catch (std::exception &e) {
        std::cout << "[ARCADE TCP SERVER]: failed to start server: " << e.what() << std::endl; 
        return false;
    }
    return true;
}

void tcp::Server::waitConnections()
{
    _acceptor.async_accept([this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket){
        if (!ec) {
            json toSend;

            std::cout << "[ARCADE TCP SERVER]: new connection detected " << socket.remote_endpoint() << std::endl;
            _clients.push_back(boost::make_shared<Connection>(socket));
            _clients.back()->readMessage();
            toSend["socketAdress"] = _clients.back()->getSocket().remote_endpoint().address().to_string();
            toSend["socketPort"] = _clients.back()->getSocket().remote_endpoint().port();
            toSend["Mesaage"] = "The client has been succesfully connected";
            toSend["StatusCode"] = CONNECTION_SUCCESS;
            _clients.back()->writeMessage(toSend.dump() + "\r\n");
        } else {
            std::cout << "[ARCADE TCP SERVER]: A connection has been refused " << ec.message() << std::endl;
        }
        waitConnections();
    });
}

void tcp::Server::update()
{
    _clients.erase(std::remove_if(
        _clients.begin(),
        _clients.end(),
        [this](const boost::shared_ptr<Connection> cli){
            return (cli->getSocket().is_open());
        }
    ), _clients.end());


    for (auto &cli: _clients) {
        _threadPool.addClassTask(Server::handleCommand, this, cli);
    }
}

void tcp::Server::handleCommand(boost::shared_ptr<Connection> &cli)
{
    std::vector<json> request = cli->getRequest();
    for (const auto &req: request) {
        if (req.find("command") != req.end()) {
            noRouteFound(cli);
            continue;
        } else if (_routes.find(req["command"]) !=  _routes.end()) {
            badCommand(cli);
            continue;
        } else {
            (this->*_routes[req["command"]])(req, cli);
        }
    }
}

void tcp::Server::badCommand(boost::shared_ptr<Connection> &con)
{
    json toSend;
    toSend["Message"] = "Command not found";
    toSend["StatusCode"] = ERROR_ARCARDE_SERVER_BAD_COMMAND;
    con->writeMessage(toSend.dump());
}

void tcp::Server::broadcastToAll(const std::string &msg)
{
    for (auto &cli: _clients)
        _threadPool.addClassTask(Connection::writeMessage, cli, msg);
}

void tcp::Server::broadcastToAll(const std::string &msg, rmh::room *salon)
{
    for (auto &cli: salon->_clients)
        _threadPool.addClassTask(Connection::writeMessage, cli, msg);
}

void tcp::Server::noRouteFound(boost::shared_ptr<Connection> &con)
{
    json toSend;
    toSend["Message"] = "The command field is missing";
    toSend["StatusCode"] = ERROR_ARCARDE_SERVER_ROUTE_NOT_FOUND;
    con->writeMessage(toSend.dump());
}