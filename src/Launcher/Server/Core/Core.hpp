#pragma once
// #include "MySqlDb.hpp"
#include "Database.hpp"
#include "TCP/server/Server.hpp"
#include "Config.h"
#include <cryptopp/osrng.h>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/base64.h>

#define TABLE "clientest"


namespace lau {
    typedef enum lerror_s {
        OK = 200,
        LOGGEDIN = 201,
        SIGNEDUP = 202,
        WRONG = 400,
        RELOG = 401,
        MISSING_ARGS = 402 
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
            int _tokenExpiration;
            std::atomic_bool _stopServer;

        private:

            //function used to add all the handlers
            void addRoute();

            //account management

            //function used for login 
            void login(const nlohmann::json &, tcp::Connection &);
            void loginToken(const nlohmann::json &, tcp::Connection &);

            //signup functions
            void signup(const nlohmann::json &, tcp::Connection &);
            void checkPseudoAvailability(const nlohmann::json &, tcp::Connection &);

            //friends function
            void postFriendsRequest(const nlohmann::json &, tcp::Connection &);
            void acceptFriendRequest(const nlohmann::json &, tcp::Connection &);


            // friends
            void sendFriendsRequest(const nlohmann::json &, tcp::Connection &);
            void searchProfile(const nlohmann::json &, tcp::Connection &);

            //

            // hash functions for passwords
            std::string hashString(const std::string &toHash);
            std::string hashString(const std::string &toHash, const std::string &st);
            std::string generateSalt(bool hex);
            void fromHashToHex(const std::string &, std::string &, int);
            void fromHashToHex(CryptoPP::SecByteBlock &, std::string &, int);
            void fromHexToHash(const std::string &, std::string &);

            //token function
            void fromHashToBase64(const std::string &, std::string &);
            void fromBase64ToHash(const std::string &, std::string &);
            std::string generateToken();
            bool verifyToken(const std::string &, const std::string &, tcp::Connection &con);

            //utils
            void sendSuccess(const nlohmann::json &);
            void sendError(const std::string &);
    };
}