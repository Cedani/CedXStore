#pragma once
#include "MySqlDb.hpp"
#include "Server.hpp"

namespace lau {
    struct Account {
        std::string pseudo;
        std::string password;
    };

    class AccountManager {
        public:
            AccountManager(dtb::IDatabase &);

            void addRoute(tcp::Server &serv);
        private:
            dtb::IDatabase & _db;
    };
}