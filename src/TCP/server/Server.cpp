#include "Server.hpp"

using nlohmann::json;

tcp::Server::Server(int port): _acceptor(_io, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)), _threadPool(10), _started(false), _nbClient(0)
{
    _threadPool.init();
}

tcp::Server::~Server()
{
    if (_started) {
        _acceptor.close();
        _io.stop();
        _threadContext.join();
    }
}

bool tcp::Server::start()
{
    try {
        _threadContext = std::thread([this](){_io.run();});
        waitConnections();
        std::cout << "[ARCADE TCP SERVER]: new server started (" << _acceptor.local_endpoint() << ")" << std::endl; 
        _started = true;
    } catch (std::exception &e) {
        std::cout << "[ARCADE TCP SERVER]: failed to start server: " << e.what() << std::endl; 
        return false;
    }
    return true;
}

void tcp::Server::addConnection()
{
    
    if (_tmpSocket.size() > 0)
        return;
    
}

void tcp::Server::waitConnections()
{
    _acceptor.async_accept([this](asio::error_code ec, asio::ip::tcp::socket socket){
        if (!ec) {
            json toSend;
            std::cout << "[ARCADE TCP SERVER]: new connection detected " << socket.remote_endpoint() << std::endl;
            {
                std::unique_lock<std::shared_mutex> lock(_mutex);
                _clients.push_back(std::make_shared<Connection>(socket));
                _clients.back()->readMessage();
                toSend["socketAdress"] = _clients.back()->getSocket().remote_endpoint().address().to_string();
                toSend["socketPort"] = _clients.back()->getSocket().remote_endpoint().port();
                toSend["Mesaage"] = "The client has been succesfully connected";
                toSend["StatusCode"] = CONNECTION_SUCCESS;
                _clients.back()->writeMessage(toSend.dump() + "\r\n");
                _nbClient += 1;
            }
                // _waiter.notify_all();
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
        [](const std::shared_ptr<Connection> cli){
            return (!cli->isSocketOpen());
        }
    ), _clients.end());
    // }
        // std::unique_lock lock(_mutex);

    _nbClient = _clients.size();
    for (auto &cli: _clients) {
        handleCommand(*cli);
    }
    // update();
}

void tcp::Server::handleCommand(Connection &cli)
{
    std::vector<json> request = cli.getRequest();

    for (int i = 0; i < request.size(); ++i) {
        if (request[i].find("command") == request[i].end()) {
            noRouteFound(cli);
            continue;
        } else if (_routes.find(request[i]["command"]) ==  _routes.end()) {
            badCommand(cli);
            continue;
        } else {
            _threadPool.addNewTask(_routes[request[i]["command"]], request[i], std::ref(cli));
            // _threadPool.addNewTask(_routes[request[i]["command"]]);
            // _threadPool.addClassTask(_routes[request[i]["command"]], this, request[i], std::ref(cli));
            // (this->*_routes[request[i]["command"]])(request[i], cli);
        }
    }
}

void tcp::Server::badCommand(Connection &con)
{
    json toSend;
    toSend["Message"] = "Command not found";
    toSend["StatusCode"] = ERROR_ARCARDE_SERVER_BAD_COMMAND;
    con.writeMessage(toSend.dump());
}

// void tcp::Server::broadcastToAll(const std::string &msg)
// {
// }

void tcp::Server::addRoute(const std::string &name, std::function<void(nlohmann::json &, Connection &)> route)
{
    _routes[name] = route;
}

void tcp::Server::noRouteFound(Connection &con)
{
    json toSend;
    toSend["Message"] = "The command field is missing";
    toSend["StatusCode"] = ERROR_ARCARDE_SERVER_ROUTE_NOT_FOUND;
    con.writeMessage(toSend.dump());
}

void tcp::Server::missingArguments(Connection &cli, const std::string &argName)
{
    json msg = {
        {"StatusCode", ERROR_ARCARDE_SERVER_BAD_ARGUMENTS},
        {"Message", "Fields " + argName + " is missing"}
    };
    cli.writeMessage(msg.dump());
}

// // void tcp::Server::createRoom(json query, Connection &cli)
// // {
// //     using namespace std::chrono_literals;
// //     std::this_thread::sleep_for(5000ms);
// //     try {
// //         if (query.find("data") == query.end())
// //             return missingArguments(cli, "data");

// //         // std::cout << "okayyyyyy" << std::endl;
// //         std::cout << query  << std::endl;
// //         if (query["data"].find("name") == query["data"].end())
// //             return missingArguments(cli, "data.name");
// //         rmh::client newClient(query["data"]["name"]);
// //         cli.writeMessage(_roomHandler.createNewRoom(4, newClient).dump());
// //     } catch (const json::out_of_range &e) {
// //         std::cout << "[ARCADE DATABASE LIB]: " << e.what() << std::endl;
// //         cli.writeMessage(json{
// //             {"code", ERROR_ARCARDE_SERVER_BAD_ARGUMENTS},
// //             {"message", e.what()}
// //         }.dump());
// //     }
// // }

// // void tcp::Server::addClientToRoom(json query, Connection &cli)
// // {
// //     if (query.find("data") == query.end())
// //         return missingArguments(cli, "data");

// //     if (query["data"].find("name") == query.end())
// //         return missingArguments(cli, "data.name");

// //     rmh::client newClient(query["data"]["name"]);
// //     if (query["data"].find("roomName") != query.end())
// //         cli.writeMessage(_roomHandler.addToRoom(std::string(query["roomHandler"]), newClient));
// //     // else if (query["data"].find("roomID") != query.end())
// //     //     cli->writeMessage(_roomHandler.addToRoom((int)(query["data"]["roomID"]), newClient));
// //     else
// //         missingArguments(cli, "data.roomName");
// // }


// // void tcp::Server::quitRoom(json query, Connection &cli)
// // {
// //     if (query.find("data") == query.end())
// //         return missingArguments(cli, "data");

// //     if (query["data"].find("name") == query.end())
// //         return missingArguments(cli, "data.name");

// //     if (query["data"].find("roomName") == query.end())
// //         return missingArguments(cli, "data.roomName");
// //     cli.writeMessage(_roomHandler.quitRoom(query["data"]["name"], std::string(query["data"]["roomName"])));
// // }