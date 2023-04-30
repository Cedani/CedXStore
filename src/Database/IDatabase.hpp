#pragma once
#include <iostream>
#include <nlohmann/json.hpp>

namespace dtb {
    typedef enum {
        INT,
        DOUBLE,
        STRING
    }valueType;
    class IDatabase {
        public:
            virtual ~IDatabase() = default;


            // connect to a Database
            // virtual bool Connect() = 0;
            virtual void init() = 0;
            // Execute a simple query, without 
            // virtual bool execute(const nlohmann::json &) = 0;
            // Execute a query where you are expecting some values
            virtual nlohmann::json executeQuery(const nlohmann::json &) = 0;
            // virtual nlohmann::json executeQuery(const std::string &) = 0;
    };
}