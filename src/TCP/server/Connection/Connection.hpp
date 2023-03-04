#pragma once

#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>
#include <memory>
#include <ErrorCode.hpp>
#include <shared_mutex>
#include <asio.hpp>
#include <atomic>
#define MSGMAX 2048

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
            char _toSend[MSGMAX];
            char _toRead[MSGMAX];
            asio::ip::tcp::socket _socket;
            std::vector<nlohmann::json> _requests;
            mutable std::shared_mutex _mutex;
            std::atomic_bool _isConnected;
    };
}