#pragma once
#include "IDatabase.hpp"
#include <mysqlx/xdevapi.h>
#include <mysqlx/common/value.h>
#include <fstream>
#include <memory>
#include <shared_mutex>
// #include <typeinfo>
// #include <typeindex>
#include <unordered_map>
#include <time.h>
#include <sstream>


namespace dtb{
    typedef enum {
        NO_COMMAND_FIELD = 400,
        COMMAND_NOT_FOUND = 404,
        OK = 200,
        SQL_EXCEPTION = 401,
    }CODE;
    class MySqlDb : public IDatabase {
        public:
            MySqlDb(const std::string &);
            ~MySqlDb() = default;

            bool Connect() final;
            bool execute(const nlohmann::json &) final;
            nlohmann::json executeQuery(const nlohmann::json &) final;
            // nlohmann::json executeQuery(const std::string &) final;
        private:
            std::string _filepath;
            std::unique_ptr<mysqlx::Session> _session;
            std::shared_mutex _mutex;
            std::unique_ptr<mysqlx::Schema> _db;
            std::unordered_map<std::string, nlohmann::json(MySqlDb::*)(const nlohmann::json &)> _functions;

        private:
            // PRIVATE FUNCTIONS
            void storeValues(nlohmann::json &, const mysqlx::abi2::r0::Row &, const mysqlx::abi2::r0::RowResult::Columns &, int size);

            // select
            nlohmann::json select(const nlohmann::json &);
            void binder(const nlohmann::json &, std::unique_ptr<mysqlx::abi2::r0::TableSelect> &);
            // nlohmann::json select(const std::string &);
            // get timestamps value
            std::string getTimeStamp(int);
    };
}