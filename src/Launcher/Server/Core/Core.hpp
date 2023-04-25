#pragma once
#include "IDatabase.hpp"
#include "Server.hpp"


namespace lau {
    class Core {
        public: 
            Core(int port);
            ~Core();


            void init();
            void run();

        private:
            int _port;
            std::unique_ptr<dtb::IDatabase> _db;
            // std::thread _serverThread;
            tcp::Server _server;

        private:

        //function used to add all the handlers
        void addRoute();

        //account management

        //function used for login 
        void login(nlohmann::json &, tcp::Connection &);

        void signup(nlohmann::json &, tcp::Connection &);
    };
}