#include "Database.hpp"

dtb::Database::Database(const std::string &filepath) : _filepath(filepath), _ssl_ctx(boost::asio::ssl::context::tls_client), _con(_ctx.get_executor(), _ssl_ctx)
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

    try
    {
        std::string line;
        while (std::getline(envF, line))
        {
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
            database);
        boost::asio::ip::tcp::resolver resol(_ctx.get_executor());
        auto endpoints = resol.resolve(host, boost::mysql::default_port_string);

        // _con.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(host), port), params);
        _con.connect(*endpoints.begin(), params);
        envF.close();
        return true;
    }
    catch (boost::mysql::error_with_diagnostics &e)
    {
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

json dtb::Database::selectLoginLauncher(std::tuple<std::string> params)
{
    try
    {
        std::string query = "select pseudo, kslt, password from clientest\n"
                            "where pseudo = ?";
        boost::mysql::statement stmt = _con.prepare_statement(query);

        boost::mysql::results res;
        _con.execute_statement(stmt, params, res);
        boost::mysql::rows row(res.rows());
        auto myRow = row.at(0);
        boost::mysql::field pseudo(myRow.at(0));
        boost::mysql::field kslt(myRow.at(1));
        boost::mysql::field password(myRow.at(2));
        return nlohmann::json{
            {"pseudo", pseudo.as_string()},
            {"kslt", kslt.as_string()},
            {"password", password.as_string()},
            {"code", OK}};
        // return nlohmann::json{
        //     {"pseudo", pseudo.as_string()},
        //     {"kslt", std::string(kslt.as_blob().begin(), kslt.as_blob().end())},
        //     {"password", std::string(password.as_blob().begin(), password.as_blob().end())},
        //     {"code", OK}};
    }
    catch (boost::mysql::error_with_diagnostics &e)
    {
        std::cerr << "Error: " << e.what() << '\n'
                  << "Server diagnostics: " << e.get_diagnostics().server_message() << std::endl;
        return nlohmann::json{
            {"message", e.get_diagnostics().server_message()},
            {"code", e.code().value()}};
    }
}