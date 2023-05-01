#pragma once

#include <iostream>
#include <boost/mysql.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/system/system_error.hpp>
#include <boost/utility/string_view.hpp>
#include <boost/mysql/blob.hpp>
#include <fstream>
#include <tuple>
#include <nlohmann/json.hpp>
#include <unordered_map>

namespace dtb
{
    typedef enum
    {
        // NO_COMMAND_FIELD = 400,
        COMMAND_NOT_FOUND = 404,
        // SQL_EXCEPTION = 401,
        OK = 200,
        // NO_RESULT = 402,
    } CODE;

    class Database
    {
    public:
        Database(const std::string &_filepath);
        ~Database();

        bool connect();

        template <typename... T>
        nlohmann::json executeQuery(const std::string &query, std::tuple<T...> params)
        {
            try
            {
                boost::mysql::statement stmt = _con.prepare_statement(query);

                boost::mysql::results res;
                _con.execute_statement(stmt, params, res);
                return nlohmann::json{
                    {"message", res.info()},
                    {"rows affected", res.affected_rows()},
                    {"code", OK}
                };
            }
            catch (boost::mysql::error_with_diagnostics &e)
            {
                std::cerr << "Error: " << e.what() << '\n'
                          << "Server diagnostics: " << e.get_diagnostics().server_message() << std::endl;
                return nlohmann::json{
                    {"message", e.get_diagnostics().server_message()},
                    {"code", e.code().value()}};
            }
        };

        nlohmann::json selectLoginLauncher(std::tuple<std::string>);

    private:
        boost::asio::io_context _ctx;
        boost::asio::ssl::context _ssl_ctx;
        boost::mysql::tcp_ssl_connection _con;
        std::string _filepath;
        // std::unordered_map<std::string, std::function<void()
        

        // void addSelectQuery();


    private:
        // void init();

        // // std::tuple<> getParameters(const nlohmann::json &params);
        // nlohmann::json insert(const std::string &);

        // template <typename... Args>
        // nlohmann::json update(const std::string &query, std::tuple<Args...> params)
        // {
        //     try
        //     {
        //         boost::mysql::statement stmt = _con.prepare_statement(query);

        //         boost::mysql::results res;
        //         _con.execute_statement(stmt, params, res);
        //         return json{
        //             {"message", res.info()},
        //             {"code", res.affected_rows()}};
        //     }
        //     catch (boost::mysql::error_with_diagnostics &e)
        //     {
        //         std::cerr << "Error: " << e.what() << '\n'
        //                   << "Server diagnostics: " << e.get_diagnostics().server_message() << std::endl;
        //         return json{
        //             {"message", e.get_diagnostics().server_message()},
        //             {"code", e.code().value()}};
        //     }
        // };

        // template <typename... Args>
        // nlohmann::json remove(const std::string &query, std::tuple<Args...>params)
        // {
        //     try
        //     {
        //         boost::mysql::statement stmt = _con.prepare_statement(query);

        //         boost::mysql::results res;
        //         _con.execute_statement(stmt, std::tuple(), res);
        //         return json{
        //             {"message", res.info()},
        //             {"code", res.affected_rows()}};
        //         // return json{}
        //     }
        //     catch (boost::mysql::error_with_diagnostics &e)
        //     {
        //         std::cerr << "Error: " << e.what() << '\n'
        //                   << "Server diagnostics: " << e.get_diagnostics().server_message() << std::endl;
        //         return json{
        //             {"message", e.get_diagnostics().server_message()},
        //             {"code", e.code().value()}};
        //     }
        // };

        // template <typename... Args>
        // nlohmann::json select(const nlohmann::json &query, std::tuple<Args...>){

        // };

        // nlohmann::json bindValues(const boost::mysql::results &, const nlohmann::json &);
    };
}