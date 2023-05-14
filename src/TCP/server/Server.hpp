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
        request_s(Connection &c, nlohmann::json j = {}): _con(c), _req(j) {};
        request_s operator=(request_s &&r) {
            // return (std::move())
            // _con = r._con;
            *this = std::move(r);
        };

        Connection &_con;
        nlohmann::json _req;
    } request;

    class Server {
        public:
            // PUBLIC FUNCTIONS

            Server(std::atomic_bool &st, int port = 47920);
            ~Server();

            bool start();
            void update();

            void addRoute(const std::string &, std::function<void(const nlohmann::json &, Connection &)>);
            void notifyStop();

        private:
            boost::thread _threadContext;
            boost::asio::io_context _io;
            boost::asio::executor_work_guard<boost::asio::io_context::executor_type> _guard;
            boost::asio::ip::tcp::acceptor _acceptor;
            std::vector<std::unique_ptr<Connection>> _clients;
            std::unordered_map<std::string, std::function<void(const nlohmann::json &, Connection &)>> _routes;
            thp::ThreadPool _threadPool;
            // rmh::RoomHandler _roomHandler;
            std::shared_mutex _mutex;
            std::condition_variable_any _waiter;
            bool _started;
            std::atomic_bool &_stop;
            std::atomic<int> _nbClient;
            // std::vector<boost::asio::ip::tcp::socket> _tmpSocket;
            thp::ThreadSafeQueue<request> _requests; 

        private:
            //add Connections
            // void addConnection();


            // ROUTES RESPONSES
            void badCommand(Connection &);
            void noRouteFound(Connection &);


        private:
            // PRIVATE FUNCTIONS
            // void broadcastToAll(const std::string &msg);
            // void broadcastToAll(const std::string &msg, rmh::room *);
            void waitConnections();
            // void handleCommand(Connection &);
            // void missingArguments(Connection &, const std::string &);
    };
}
