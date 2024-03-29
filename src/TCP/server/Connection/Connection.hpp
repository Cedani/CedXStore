#pragma once

#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>
#include <memory>
#include <ErrorCode.hpp>
#include <shared_mutex>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <atomic>

// #define MSGMAXWRITE 2048
#define MSGMAXR 1024
#define MSGMAXW 2048

namespace tcp {
    class Connection {
        public:
            Connection(boost::asio::ip::tcp::socket &socket, const std::function<void(const nlohmann::json &, Connection &)> &);
            ~Connection();

            void readMessage();
            void writeMessage(const std::string &message);
            const boost::asio::ip::tcp::socket &getSocket() const;
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
            boost::asio::ip::tcp::socket _socket;
            std::vector<nlohmann::json> _requests;
            mutable std::shared_mutex _mutex;
            std::atomic_bool _isConnected;

            std::function<void(const nlohmann::json &, Connection &)> addRequest;
    };
}