#pragma once

#include "Connection/Connection.hpp"
#include <unordered_map>
#include <shared_mutex>
#include "ThreadPool.hpp"
// #include 
// #include "RoomHandler/RoomHandler.hpp"

namespace tcp
{
    typedef struct request_s {
        request_s(std::shared_ptr<Connection>c = nullptr, nlohmann::json j = {}): _con(c), _req(j) {};
        std::shared_ptr<Connection> _con;
        nlohmann::json _req;
    } request;
    class Server : public std::enable_shared_from_this<Server> {
        public:
            // PUBLIC FUNCTIONS

            Server(int port = 47920);
            ~Server();

            bool start();
            void update();

            void addRoute(const std::string &, std::function<void(nlohmann::json, Connection &)>);

        private:
            std::thread _threadContext;
            asio::io_context _io;
            asio::executor_work_guard<asio::io_context::executor_type> _guard;
            asio::ip::tcp::acceptor _acceptor;
            std::vector<std::shared_ptr<Connection>> _clients;
            std::unordered_map<std::string, std::function<void(nlohmann::json, Connection &)>> _routes;
            thp::ThreadPool _threadPool;
            // rmh::RoomHandler _roomHandler;
            std::shared_mutex _mutex;
            std::condition_variable_any _waiter;
            bool _started;
            std::atomic<int> _nbClient;
            std::vector<asio::ip::tcp::socket> _tmpSocket;
            thp::ThreadSafeQueue<request> _requests; 

        private:
            //add Connections
            void addConnection();


            // ROUTES RESPONSES
            void badCommand(Connection &);
            void noRouteFound(Connection &);


        private:
            // PRIVATE FUNCTIONS
            // void broadcastToAll(const std::string &msg);
            // void broadcastToAll(const std::string &msg, rmh::room *);
            void waitConnections();
            void handleCommand(Connection &);
            void missingArguments(Connection &, const std::string &);
    };
}
