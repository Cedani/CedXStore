#include "Core/Core.hpp"
#include "MySqlDb.hpp"
#include "config.h"
#include <sstream>

using nlohmann::json;

int main(void)
{
    std::unique_ptr<dtb::IDatabase> _db = std::make_unique<dtb::MySqlDb>(LIB_DTB_ENV_PATH);
    _db->init();
    // lau::Core program(47920);
    std::stringstream command;

    command << "USE ArcadeDB;\n"
    "insert into clientest (pseudo, password, kslt)\n"
    "values (\"testeurdu34\", \"1234\", \"1234\")";

    json query{
        {"command", "insert"},
        {"query", command.str()}
    };


    auto tmp = _db->executeQuery(query);

    std::cout << tmp << std::endl;
    // std::cout << "tmp" << std::endl;
    // program.init();
    // program.run();
}