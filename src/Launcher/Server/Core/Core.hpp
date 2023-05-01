#pragma once
// #include "MySqlDb.hpp"
#include "Database.hpp"
#include "TCP/server/Server.hpp"
#include "Config.h"
#include <cryptopp/osrng.h>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>

#define TABLE "clientest"


namespace lau {
    typedef enum lerror_s {
        OK = 200,
        WRONG = 400
    }lerror;
    class Core {
        public: 
            Core(int port);
            // ~Core();



            void init();
            void run();

        private:
            int _port;
            dtb::Database _db;
            tcp::Server _server;
            void missingArguments(tcp::Connection &, const std::string &);

        private:

            //function used to add all the handlers
            void addRoute();

            //account management

            //function used for login 
            void login(const nlohmann::json &, tcp::Connection &);

            void signup(const nlohmann::json &, tcp::Connection &);

            // hash functions for passwords
            std::string hashString(const std::string &toHash);
            std::string hashString(const std::string &toHash, const std::string &st);
            std::string generateSalt();
            void fromHashToHex(const std::string &, std::string &, int);
            void fromHashToHex(CryptoPP::SecByteBlock &, std::string &, int);
            void fromHexToHash(const std::string &, std::string &);

            //utils
            void sendSuccess(const nlohmann::json &);
            void sendError(const std::string &);
    };
}