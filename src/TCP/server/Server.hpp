#pragma once

#include "Connection/Connection.hpp"
#include <boost/make_shared.hpp>
#include <unordered_map>
#include <shared_mutex>
#include "ThreadPool.hpp"
#include "RoomHandler/RoomHandler.hpp"

using nlohmann::json;

namespace tcp
{

    class Server : public std::enable_shared_from_this<Server> {
        public:
            // PUBLIC FUNCTIONS
            Server(int port);
            ~Server();

            bool start();
            void update();


        private:
            std::thread _threadContext;
            asio::io_context _io;
            asio::ip::tcp::acceptor _acceptor;
            std::vector<std::shared_ptr<Connection>> _clients;
            std::unordered_map<std::string, void (Server::*)(json, Connection &)> _routes;
            thp::ThreadPool _threadPool;
            rmh::RoomHandler _roomHandler;
            std::shared_mutex _mutex;
            std::condition_variable_any _waiter;


        private:
            // ROUTES RESPONSES
            void badCommand(Connection &);
            void noRouteFound(Connection &);
            void createRoom(json, Connection &);
            void addClientToRoom(json , Connection &);
            void quitRoom(json, Connection &);


        private:
            // PRIVATE FUNCTIONS
            void broadcastToAll(const std::string &msg);
            // void broadcastToAll(const std::string &msg, rmh::room *);
            void waitConnections();
            void handleCommand(Connection &);
            void missingArguments(Connection &, const std::string &);
    };
}
