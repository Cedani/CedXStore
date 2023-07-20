#pragma once
#include <iostream>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <future>
#include "Config.h"
#define MSGMAXW 1024
#define MSGMAXR 2048


namespace lau {
    class Client {
        public:
            Client();

            bool connectToServer();
            nlohmann::json readMessage();
            bool writeMessage(const std::string &);
            // static std::async 
        private:
            boost::asio::io_context _ctx;
            boost::asio::ip::tcp::socket _sock;
            char _read[MSGMAXR];
            char _write[MSGMAXW];
    };
}