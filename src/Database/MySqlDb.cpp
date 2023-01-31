#include "MySqlDb.hpp"

using nlohmann::json;

dtb::MySqlDb::MySqlDb(const std::string &filepath): _filepath(filepath), _session(nullptr)
{
    _functions["select"] = &MySqlDb::select;
    _functions["insert"] = &MySqlDb::insert;
}

bool dtb::MySqlDb::Connect()
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
            // auto tmp = _session->sql("USE " + database).execute();
            // if (tmp.hasData())
            //     std::cout << "has data" << std::endl;
        }
        std::cout << "HOST: " << host << " PORT " << port << " USER " << user << " PASSWORD " << password << " DATABASE " << database << std::endl; 
        _session = std::make_unique<mysqlx::Session>(mysqlx::Session(host, port, user, password));
        _db = std::make_unique<mysqlx::Schema>(_session->getSchema(database));
        envF.close();
    } catch (const mysqlx::Error &e) {
        std::cout << "[ARCADE DATABASE LIB]: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

bool dtb::MySqlDb::execute(const json &query)
{
    return (true);
}

json dtb::MySqlDb::executeQuery(const json &query)
{
    if (query.find("command") == query.end())
        return (json{ 
            {"code", NO_COMMAND_FIELD},
            {"message","[ARCADE DATABASE LIB]: no fields command in given json"}
        });
    if (_functions.find(query["command"]) == _functions.end())
        return(json{
            {"code", COMMAND_NOT_FOUND},
            {"message", "[ARCADE DATABASE LIB]: command not found"}
        });
    return ((this->*_functions[query["command"]])(query));
}

// json dtb::MySqlDb::executeQuery(const std::string &query)
// {
//     return (select(query));
// }

void dtb::MySqlDb::storeValues(json &rowJson, const mysqlx::abi2::r0::Row &row, const mysqlx::abi2::r0::RowResult::Columns &columns, int size)
{
    for (int i = 0; i < size; ++i) {
        std::string label;
        switch(row[i].getType()) {
            case mysqlx::abi2::r0::Value::Type::ARRAY:
                rowJson[columns[i].getColumnLabel()] = row[i].getRawBytes();
                break;
            case mysqlx::abi2::r0::Value::Type::BOOL:
                rowJson[columns[i].getColumnLabel()] = row[i].get<bool>();
                break;
            case mysqlx::abi2::r0::Value::Type::DOCUMENT:
                rowJson[columns[i].getColumnLabel()] = row[i];
                break;
            case mysqlx::abi2::r0::Value::Type::DOUBLE:
                rowJson[columns[i].getColumnLabel()] = row[i].get<double>();
                break;
            case mysqlx::abi2::r0::Value::Type::FLOAT:
                rowJson[columns[i].getColumnLabel()] = row[i].get<float>();
                break;
            case mysqlx::abi2::r0::Value::Type::INT64:
                label = columns[i].getColumnLabel();
                if (label.find("UNIX_TIMESTAMP") != label.npos)
                    rowJson[label] = getTimeStamp(row[i].get<int>());
                else
                    rowJson[columns[i].getColumnLabel()] = row[i].get<int64_t>();
                break;
            case mysqlx::abi2::r0::Value::Type::RAW:
                rowJson[columns[i].getColumnLabel()] = (int)row[i];
                break;
            case mysqlx::abi2::r0::Value::Type::STRING:
                rowJson[columns[i].getColumnLabel()] = row[i];
                break;
            default:
                rowJson[columns[i].getColumnLabel()] = "null";
                break;
        }
    }
}

json dtb::MySqlDb::select(const json &query)
{
    try {
        json result;
        mysqlx::Table table = _db->getTable(std::string(query["table"]));
        std::unique_ptr<mysqlx::abi2::r0::TableSelect> request;
        std::string typeSelect(query["fields"].type_name());

        // whether to select some fields, or all, or just one fields
        if (query["fields"].is_array())
            request = std::make_unique<mysqlx::abi2::r0::TableSelect>(table.select(std::forward<std::vector<std::string>>(query["fields"])));
        else
            request = std::make_unique<mysqlx::abi2::r0::TableSelect>(table.select(std::string(query["fields"])));
        
        // whether there is a where
        if (query.find("where") != query.end())
            request->where(std::string(query["where"]));

        //whether there is an order by
        if (query.find("orderBy") != query.end())
            request->orderBy(std::string(query["orderBy"]));

        binder(query, request);
        auto resultRaw = request->execute();
        auto &columns = resultRaw.getColumns();

        for (auto row = resultRaw.fetchOne(); row; row = resultRaw.fetchOne()) {
            json rowJson;
            storeValues(rowJson, row, columns, resultRaw.getColumnCount());
            result["data"].push_back(rowJson);
        }
        return (result);
    } catch (const mysqlx::Error &e) {
        std::cout << "[ARCADE DATABASE LIB]: " << e.what() << std::endl;
        return (json{
            {"code", SQL_EXCEPTION},
            {"message", e.what()}
        });
    } catch (const json::out_of_range &e) {
        std::cout << "[ARCADE DATABASE LIB]: " << e.what() << std::endl;
        return (json{
            {"code", e.id},
            {"message", e.what()}
        });
    }
}

void dtb::MySqlDb::binder(const json &query, std::unique_ptr<mysqlx::abi2::r0::TableSelect> &request)
{
    if (query.find("data") == query.end())
        return;
    for (const auto &v: query["data"]) {
        std::cout << "query is " << v << std::endl;
        if (v["value"].is_boolean())
            request->bind(std::string(v["label"]), (bool)v["value"]);
        else if (v["value"].is_string())
            request->bind(std::string(v["label"]), std::string(v["value"]));
        else if (v["value"].is_number_integer())
            request->bind(std::string(v["label"]), (int)v["value"]);
        else if (v["value"].is_number_unsigned())
            request->bind(std::string(v["label"]), (unsigned int)v["value"]);
        else if (v["value"].is_number_float())
            request->bind(std::string(v["label"]), (double)v["value"]);
    }
}

std::string dtb::MySqlDb::getTimeStamp(int v)
{
    time_t timeVal = v;
    auto timeStruct = localtime(&timeVal);
    std::stringstream out;
    out << std::put_time(timeStruct, "%d/%m/%Y %H:%M:%S");
    return (out.str()); 
}


json dtb::MySqlDb::insert(const json &query)
{
    try {
        mysqlx::Table table = _db->getTable(std::string(query["table"]));
        std::unique_ptr<mysqlx::abi2::r0::TableInsert> request;

        if (query["fields"].is_array())
            request = std::make_unique<mysqlx::abi2::r0::TableInsert>(table.insert(std::forward<std::vector<std::string>>(query["fields"])));
        else
            request = std::make_unique<mysqlx::abi2::r0::TableInsert>(table.insert(std::string(query["fields"])));
        // binder(query, request);
        request->values(std::forward<std::vector<std::string>>(query["data"]));
        auto resultRequest = request->execute();
        std::cout << resultRequest.getAffectedItemsCount() << std::endl;
        return (json{
            {"code", OK},
            {"Count", resultRequest.getAffectedItemsCount()},
            {"Message", "succesfully inserted valuee"}
        });
    } catch (const mysqlx::Error &e) {
        std::cout << "[ARCADE DATABASE LIB]: " << e.what() << std::endl;
        return (json{
            {"code", SQL_EXCEPTION},
            {"message", e.what()}
        });
    } catch (const json::out_of_range &e) {
        std::cout << "[ARCADE DATABASE LIB]: " << e.what() << std::endl;
        return (json{
            {"code", e.id},
            {"message", e.what()}
        });
    } catch (const json::type_error &e) {
        std::cout << "[ARCADE DATABASE LIB]: " << e.what() << std::endl;
        return (json{
            {"code", e.id},
            {"message", e.what()}
        });
    }
}

void dtb::MySqlDb::binder(const json &query, std::unique_ptr<mysqlx::abi2::r0::TableInsert> &request)
{
    // request->rows()
    if (query.find("data") == query.end())
        return;
    std::cout << "request is (((((((" << query["data"] << ")))))))" << std::endl;
    for (int i = 0; i < query["data"].size(); ++i) {
        const json & v = query["data"][i];
        std::cout << "request is (((((((" << v << ")))))))" << std::endl;
        if (v.is_boolean())
            request->values(i, (bool)v);
        else if (v.is_string())
            request->values(i, std::string(v));
        else if (v.is_number_integer())
            request->values(i, (int)v);
        else if (v.is_number_unsigned())
            request->values(i, (unsigned int)v);
        else if (v.is_number_float())
            request->values(i, (double)v);
    }
}