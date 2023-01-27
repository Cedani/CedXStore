#include "ThreadPool.hpp"
#include <chrono>
#include <string>
#include <fstream>


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
    // std::ofstream tmp("tmp1nb" + std::to_string(num));

    // std::this_thread::sleep_for(2000ms);

    // std::cout << "here" << std::endl;
    // std::cout.flush();
    for (int i = 0; i < num; ++i) {
        std::cout << "thread here " << std::this_thread::get_id() << " " << i << " for max " << num << std::endl; 
        // x += i;
    }
    // std::cout << "finished" << std::endl;
    // finished();
}

int main(void)
{
    // srand((unsigned int)time(NULL));
    using namespace std::chrono_literals;
    thp::ThreadPool test(4);
    test.init();
    std::cout << "length: " << test.length() << std::endl;
    // test.addNewTask(testInt, 50000);
    // test.addNewTask(testInt, 50000);
    // test.addNewTask(testInt, 50000);
    // test.addNewTask(testInt, 50000);
    // test.addNewTask(testInt, 50000);
    // std::cout << "> ";
    // std::string cmd;
    while (1);
    
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
}
