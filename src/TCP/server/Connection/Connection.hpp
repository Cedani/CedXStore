#pragma once

#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>
#include <memory>
#include <ErrorCode.hpp>
#include <shared_mutex>
#include <asio.hpp>
#include <atomic>
// #define MSGMAXWRITE 2048
#define MSGMAXR 1024
#define MSGMAXW 2048

namespace tcp {
    class Connection: public std::enable_shared_from_this<Connection> {
        public:
            Connection(asio::ip::tcp::socket &socket);
            ~Connection();

            void readMessage();
            void writeMessage(const std::string &message);
            const asio::ip::tcp::socket &getSocket() const;
            void printMessage(const std::string &msg);
            void printDisconnection();
            const std::vector<nlohmann::json> getRequest();
            std::size_t getRequestSize() const;
            bool operator==(const Connection &);
            bool isSocketOpen() const;

        private:
            void parseRead();

            char _toSend[MSGMAXW];
            char _toRead[MSGMAXR];
            bool _messageEnd = false;
            std::string _readBuffer;
            std::string _finishedBuffer;
            asio::ip::tcp::socket _socket;
            std::vector<nlohmann::json> _requests;
            mutable std::shared_mutex _mutex;
            std::atomic_bool _isConnected;
    };
    typedef struct request_s {
        Connection &_con;
        // std::string 
    } request;
}