#include "Database.hpp"

dtb::Database::Database(const std::string &filepath): _filepath(filepath), _ssl_ctx(boost::asio::ssl::context::tls_client), _con(_ctx.get_executor(), _ssl_ctx)
{

}

using nlohmann::json;

dtb::Database::~Database()
{
    _con.close();
}

bool dtb::Database::connect()
{
    std::string host;
    std::string user;
    std::string password;
    std::string database;
    int port = 0;
    std::ifstream envF(_filepath);

    try {
         std::string line;
        while (std::getline(envF, line)) {
            if (line.find("HOST") != line.npos)
                host = line.substr(line.find("= ") + 2);
            else if (line.find("USER") != line.npos)
                user = line.substr(line.find("= ") + 2);
            else if (line.find("PASSWORD") != line.npos)
                password = line.substr(line.find("= ") + 2);
            else if (line.find("DATABASE") != line.npos)
                database = line.substr(line.find("= ") + 2);
            else if (line.find("PORT") != line.npos)
                port = atoi(line.substr(line.find("= ") + 2).c_str());
        }
        
        boost::mysql::handshake_params params(
            user,
            password,
            database
        );
        boost::asio::ip::tcp::resolver resol(_ctx.get_executor());
        auto endpoints = resol.resolve(host, boost::mysql::default_port_string);

        // _con.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(host), port), params);
        _con.connect(*endpoints.begin(), params);
        envF.close();
        return true;
    } catch (boost::mysql::error_with_diagnostics &e) {
        std::cerr << "Error: " << e.what() << '\n'
                  << "Server diagnostics: " << e.get_diagnostics().server_message() << std::endl;
        return false;
    }
}

// json dtb::Database::insert(const std::string &query)
// {
//     try {
//         boost::mysql::statement stmt = _con.prepare_statement(query);

//         boost::mysql::results res;
//         _con.execute_statement(stmt, std::tuple(), res);
//         return json{
//             {"message", res.info()},
//             {"code", res.affected_rows()}
//         };
//         // return json{}
//     } catch (boost::mysql::error_with_diagnostics &e) {
//         std::cerr << "Error: " << e.what() << '\n'
//                   << "Server diagnostics: " << e.get_diagnostics().server_message() << std::endl;
//         return json{
//             {"message", e.get_diagnostics().server_message()},
//             {"code", e.code().value()}
//         };
//     }
// }