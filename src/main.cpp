#include "ThreadPool.hpp"
#include <chrono>
#include <string>
#include <fstream>
#include "MySqlDb.hpp"
#include "Config.h"
#include <time.h>


using nlohmann::json;

class testThreadPool {
    public:
        testThreadPool(const std::string &name): _name(name) {

        };
        ~testThreadPool() = default;

        void testInt(int num, int num1) {
            std::cout << std::this_thread::get_id() << " result is: " << num + num1 << std::endl;
        }
    private:
        std::string _name;
};

void testInt(int num)
{
    using namespace std::chrono_literals;
    std::ofstream tmp("tmp1nb" + std::to_string(num));

    // std::this_thread::sleep_for(2000ms);

    // std::cout << "here" << std::endl;
    // std::cout.flush();
    if (!tmp.is_open())
        std::cout << "thread here " << std::this_thread::get_id() << "not opened" << std::endl;
    for (int i = 0; i < num; ++i) {
        tmp << "thread here " << std::this_thread::get_id() << " " << i << " for max " << num << std::endl; 
        // x += i;
    }
    // std::cout << "finished" << std::endl;
    // finished();
}


int main(void)
{
    // srand((unsigned int)time(NULL));
    using namespace std::chrono_literals;
    std::unique_ptr<dtb::IDatabase> sqldtb(std::make_unique<dtb::MySqlDb>(LIB_DTB_ENV_PATH));

    sqldtb->Connect();

    std::vector<std::string> fields;
    std::vector<std::string> fields2;
    std::vector<json> data;
    std::vector<std::string> data2;

    data.push_back({
        {"label", "param1"},
        {"value", 7}
    });

    fields2.push_back("Pseudo");
    fields2.push_back("CPassword");
    fields2.push_back("testInt");


    data2.push_back("testeurdu41");
    data2.push_back("24252627");
    data2.push_back("15");

    fields.push_back("Pseudo");
    fields.push_back("Tag");
    fields.push_back("CPassword");
    fields.push_back("UNIX_TIMESTAMP(DateCreation)");


    nlohmann::json tmp;
    tmp["command"] = "select";
    tmp["table"] = "clienTest";
    // tmp["where"] = "Tag = :param1";
    tmp["fields"] = fields;
    tmp["orderBy"] = "Tag Desc";
    tmp["data"] = data;

    json insertJson;
    insertJson["command"] = "insert";
    insertJson["table"] = "clienTest";
    insertJson["fields"] = fields2;
    insertJson["data"] = data2;
    auto result1 = sqldtb->executeQuery(insertJson);
    // std::cout << result1 << std::endl;
    auto result = sqldtb->executeQuery(tmp);
    std::cout << result << std::endl;
    // const char test[] = {15,1,30,19,56,7};
    // std::cout << result["data"][0]["DateCreation"].type_name() << std::endl;;
    // time_t test = result["data"][0]["UNIX_TIMESTAMP(`DateCreation`)"];
    // auto tmpTime = localtime(&test);
    // std::cout << std::put_time(tmpTime, "%d-%m-%Y %H-%M-%S") << std::endl;

    
    // std::cout << std::string(test, sizeof(test)) << std::endl;
    // auto tmpStr = std::basic_string<unsigned char>(test, sizeof(test));

    // for (const auto &c: tmpStr)
    //     printf("%c", c);
    // tmp["fields"] =
    // sqldtb->executeQuery();

    // bool tmp = sqldtb.execute("INSERT INTO clienttest (Pseudo, Firstname, CPassword) values ('Tester', 'Tester', '1234')");
    // if (tmp) {
    //     auto res = sqldtb.executeQuery("select * from clienttest");
    //     std::cout << "Tag\t\tPseudo\t\tFirstName\t\tCPassword\t\tavatar\\DateCreation\t\tUtimestamp" << std::endl;
    //     std::cout << res->getString("Tag") << "\t\t" << res->getString("Pseudo") << "\t\t" << res->getString("Firstname") << "\t\t" << res->getString("CPassword") << "\t\t" <<res->getString("avatar") << "\t\t" << res->getString("DateCreation") << "\t\t" <<res->getString("Utimestamp") << std::endl;
    // }
    // dtb::Database
    // thp::ThreadPool test(4);
    // test.init();
    // std::cout << "length: " << test.length() << std::endl;
    // test.addNewTask(testInt, 10000);
    // test.addNewTask(testInt, 20000);
    // test.addNewTask(testInt, 30000);
    // test.addNewTask(testInt, 40000);
    // test.addNewTask(testInt, 50000);
    // // std::cout << "> ";
    // // std::string cmd;
    // while (1);
    
    // while(std::getline(std::cin, cmd)) {
    //     if (cmd.find("terminate") != cmd.npos) {
    //         test.terminate();
    //         return (EXIT_SUCCESS);
    //     }
    //     if (cmd.find("la") != cmd.npos)
    //         test.addNewTask(testInt, atoi(cmd.substr(cmd.find(" ") + 1).c_str()));
    //     if (cmd.find("cancel") != cmd.npos) {
    //         test.cancel();
    //         return (EXIT_SUCCESS);
    //     }
    //     std::cout << std::endl << "> ";
    // };

    // nlohmann::json tmp = {
    //     {"value", 15.8f}
    // };

    // std::cout << tmp["value"].is_number_float() << std::endl;
}
