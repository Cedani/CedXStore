#include "MySqlDb.hpp"

using nlohmann::json;

dtb::MySqlDb::MySqlDb(const std::string &filepath): _filepath(filepath), _driver(nullptr)
{
}

void dtb::MySqlDb::init()
{
    
    std::ifstream envF(_filepath);
    std::string line;

    while (std::getline(envF, line)) {
        if (line.find("HOST") != line.npos)
            _hostName = line.substr(line.find("= ") + 2);
        else if (line.find("USER") != line.npos)
            _user = line.substr(line.find("= ") + 2);
        else if (line.find("PASSWORD") != line.npos)
            _password = line.substr(line.find("= ") + 2);
    }
    std::cout << _hostName << " " << _user << " " << _password << std::endl;
    // if (sql::Connection::)
    
    _driver = sql::mysql::get_mysql_driver_instance();
    // std::cout << _driver->getMajorVersion() << std::endl;
}

json dtb::MySqlDb::executeQuery(const json &query)
{
    if (query["command"] == "insert")
        return insert(query["query"].dump());
    if (query["command"] == "remove")
        return remove(query["query"].dump());
    if (query["command"] == "update")
        return update(query["query"].dump());
    if (query["command"] == "select")
        return select(query["query"]);
    return json{
        {"code", COMMAND_NOT_FOUND},
        {"Message", "command not found"}
    };
}

json dtb::MySqlDb::insert(const std::string &query)
{
    try {
        sql::Connection *con = _driver->connect(_hostName, _user, _password);
        // sql::Connection *con = _driver->connect(_hostName, _user, _password);
        std::cout << "here" << std::endl;
        sql::Statement *req = con->createStatement();
        sql::ResultSet *res = req->executeQuery(query);


        bool rowsInserted = res->rowInserted();
        delete res;
        delete req;
        delete con;
        return json{
            {"code", OK},
            {"Inserted", rowsInserted},
            // {"message", "Values succesfully inserted"}
        };
    } catch (sql::SQLException &e) {
        std::cout << "[DATABASE ERROR]: in " << __FILE__ <<  " (function " << __FUNCTION__ << " line " << __LINE__ << ")" << std::endl;
        std::cout << "ERROR: " << e.what() << std::endl;
        std::cout << "CODE: " << e.getErrorCode() << std::endl;
        std::cout << "SQLSTATE: " << e.getSQLState() << std::endl;
        return json{
            {"code", e.getErrorCode()},
            {"message", e.what()}
        };
    }
}

json dtb::MySqlDb::update(const std::string &query)
{
    try {
        sql::Connection *con = _driver->connect(_hostName, _user, _password);
        sql::Statement *req = con->createStatement();
        sql::ResultSet *res = req->executeQuery(query);


        bool rowUpdated = res->rowUpdated();
        delete res;
        delete req;
        delete con;
        return json{
            {"code", OK},
            {"Updated", rowUpdated},
            // {"message", "Values succesfully inserted"}
        };
    } catch (sql::SQLException &e) {
        std::cout << "[DATABASE ERROR]: in " << __FILE__ <<  " (function " << __FUNCTION__ << " line " << __LINE__ << ")" << std::endl;
        std::cout << "ERROR: " << e.what() << std::endl;
        std::cout << "CODE: " << e.getErrorCode() << std::endl;
        std::cout << "SQLSTATE: " << e.getSQLState() << std::endl;
        return json{
            {"code", e.getErrorCode()},
            {"message", e.what()}
        };
    }
}

json dtb::MySqlDb::remove(const std::string &query)
{
    try {
        sql::Connection *con = _driver->connect(_hostName, _user, _password);
        sql::Statement *req = con->createStatement();
        sql::ResultSet *res = req->executeQuery(query);


        bool rowDeleted = res->rowDeleted();
        delete res;
        delete req;
        delete con;
        return json{
            {"code", OK},
            {"Deleted", rowDeleted},
            // {"message", "Values succesfully inserted"}
        };
    } catch (sql::SQLException &e) {
        std::cout << "[DATABASE ERROR]: in " << __FILE__ <<  " (function " << __FUNCTION__ << " line " << __LINE__ << ")" << std::endl;
        std::cout << "ERROR: " << e.what() << std::endl;
        std::cout << "CODE: " << e.getErrorCode() << std::endl;
        std::cout << "SQLSTATE: " << e.getSQLState() << std::endl;
        return json{
            {"code", e.getErrorCode()},
            {"message", e.what()}
        };
    }
}

json dtb::MySqlDb::select(const json &query)
{
    try {
        sql::Connection *con = _driver->connect(_hostName, _user, _password);
        sql::Statement *req = con->createStatement();
        sql::ResultSet *res = req->executeQuery(query["query"].get<std::string>());


        bool rowDeleted = res->rowDeleted();
        json values = bindSelectValues(res, query);
        delete res;
        delete req;
        delete con;
        return values;
    } catch (sql::SQLException &e) {
        std::cout << "[DATABASE ERROR]: in " << __FILE__ <<  " (function " << __FUNCTION__ << " line " << __LINE__ << ")" << std::endl;
        std::cout << "ERROR: " << e.what() << std::endl;
        std::cout << "CODE: " << e.getErrorCode() << std::endl;
        std::cout << "SQLSTATE: " << e.getSQLState() << std::endl;
        return json{
            {"code", e.getErrorCode()},
            {"message", e.what()}
        };
    }
}

json dtb::MySqlDb::bindSelectValues(sql::ResultSet *res, const json &query)
{
    json ret;
    for (int i = 0; i < query["fields"].size() && res->next(); ++i) {
        switch(query["fields"][i]["type"].get<int>()) {
            case INT:
                ret[query["fields"][i]["label"]] = res->getInt(query["fields"][i]["label"]);
                break;
            case DOUBLE:
                ret[query["fields"][i]["label"]] = res->getDouble(query["fields"][i]["label"]);
                break;
            case STRING:
                ret[query["fields"][i]["label"]] = res->getString(query["fields"][i]["label"]);
                break;
            default:
                ret[query["fields"][i]["label"]] = res->getInt(query["fields"][i]["label"]);
                break;
        }
    }
    return ret;
}