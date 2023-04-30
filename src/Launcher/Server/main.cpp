// #include "Core/Core.hpp"
// #include "MySqlDb.hpp"
#include "Database.hpp"
#include "Config.h"
#include <sstream>

using nlohmann::json;

int main(void)
{
    dtb::Database db(LIB_DTB_ENV_PATH);
    std::cout << "here" << std::endl;
    // std::unique_ptr<dtb::IDatabase> _db = std::make_unique<dtb::MySqlDb>(LIB_DTB_ENV_PATH);
    // _db->init();
    // lau::Core program(47920);
    std::string command = "insert into clientest (pseudo, password, kslt)\n"
    "values (?, ?, ?)";

    if (!db.connect())
        exit(EXIT_FAILURE);

    auto tmp = db.executeQuery(command, std::make_tuple("testeurdu34", "1234", "1234"));

    // json query{
    //     {"command", "insert"},
    //     {"query", command.str()}
    // };


    // auto tmp = _db->executeQuery(query);

    std::cout << tmp << std::endl;
    // std::cout << "tmp" << std::endl;
    // program.init();
    // program.run();
}