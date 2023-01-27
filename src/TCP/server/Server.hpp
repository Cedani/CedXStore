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
    class Server : public boost::enable_shared_from_this<Server> {
        public:
            // PUBLIC FUNCTIONS
            Server(int port);
            ~Server();

            bool start();
            void update();


        private:
            boost::thread _threadContext;
            boost::asio::io_context _io;
            boost::asio::ip::tcp::acceptor _acceptor;
            std::vector<boost::shared_ptr<Connection>> _clients;
            std::unordered_map<std::string, void (Server::*)(const json &, boost::shared_ptr<Connection> &)> _routes;
            thp::ThreadPool _threadPool;
            rmh::RoomHandler _roomHandler;
            std::shared_mutex _mutex;


        private:
            // ROUTES RESPONSES
            void badCommand(boost::shared_ptr<Connection> &);
            void noRouteFound(boost::shared_ptr<Connection> &);
            void createRoom(const json &, boost::shared_ptr<Connection> &);
            void addClientToRoom(const json &, boost::shared_ptr<Connection> &);
            void quitRoom(const json &, boost::shared_ptr<Connection> &);
            void deleteRoom(const json &, boost::shared_ptr<Connection> &);


        private:
            // PRIVATE FUNCTIONS
            void broadcastToAll(const std::string &msg);
            void broadcastToAll(const std::string &msg, rmh::room *);
            void waitConnections();
            void handleCommand(boost::shared_ptr<Connection> &);
    };
}
