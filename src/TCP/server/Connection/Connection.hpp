#pragma once
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>
#include <ErrorCode.hpp>
#define MSGMAX 10240

namespace tcp {
    class Connection: public boost::enable_shared_from_this<Connection> {
        public:
            Connection(boost::asio::ip::tcp::socket &socket);
            ~Connection();

            void readMessage();
            void writeMessage(const std::string &message);
            const boost::asio::ip::tcp::socket &getSocket() const;
            void printMessage(const std::string &msg);
            void printDisconnection();
            const std::vector<nlohmann::json> getRequest();
            bool operator==(const Connection &);
        private:
            char _toSend[MSGMAX];
            char _toRead[MSGMAX];
            boost::asio::ip::tcp::socket _socket;
            std::vector<nlohmann::json> _requests;
    };
}