#pragma once
#include <jdbc/mysql_connection.h>
#include <jdbc/mysql_driver.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include "IDatabase.hpp"
#include <fstream>
#include <jdbc/mysql_error.h>
#include <jdbc/cppconn/connection.h>
#include <jdbc/cppconn/driver.h>

namespace dtb {
    typedef enum {
        // NO_COMMAND_FIELD = 400,
        COMMAND_NOT_FOUND = 404,
        // SQL_EXCEPTION = 401,
        OK = 200,
        // NO_RESULT = 402,
    }CODE;
    class MySqlDb: public IDatabase {
        public:
            MySqlDb(const std::string &);

            nlohmann::json executeQuery(const nlohmann::json &) final;
        private:

            // memebers
            sql::mysql::MySQL_Driver *_driver;
            std::string _filepath;
            std::string _hostName;
            std::string _user;
            std::string _password;
            std::unordered_map<std::string, nlohmann::json(MySqlDb::*)(const nlohmann::json &)> _functions;
    
            // methods
            void init();

            nlohmann::json insert(const std::string &);

            nlohmann::json update(const std::string &);

            nlohmann::json remove(const std::string &);

            nlohmann::json select(const nlohmann::json &query); 
    
            nlohmann::json bindSelectValues(sql::ResultSet *, const nlohmann::json &);
    };
}