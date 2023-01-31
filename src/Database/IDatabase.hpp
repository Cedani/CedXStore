#pragma once
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>

namespace dtb {
    class IDatabase {
        public:
            virtual ~IDatabase() = default;


            // connect to a Database
            virtual bool Connect() = 0;
            // Execute a simple query, without 
            virtual bool execute(const nlohmann::json &) = 0;
            // Execute a query where you are expecting some values
            virtual nlohmann::json executeQuery(const nlohmann::json &) = 0;
            // virtual nlohmann::json executeQuery(const std::string &) = 0;
    };
}