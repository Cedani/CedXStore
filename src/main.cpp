#include "ThreadPool/ThreadPool.hpp"
#include <chrono>
#include <string>
#include <fstream>
// #include "MySqlDb.hpp"
#include "Server.hpp"
#include "Config.h"
#include <nlohmann/json.hpp>
#include <time.h>
#include "myTimer.hpp"

using nlohmann::json;

class testThreadPool {
    public:
        testThreadPool(const std::string &name): _name(name) {

        };
        ~testThreadPool() = default;

        void testInt(int num, const testThreadPool &tmp1) {
            // std::ofstream tmp("tmp1nb" + std::to_string(num));
            std::cout << std::this_thread::get_id() << " result is: " << tmp1._name << std::endl;
            // tmp.close();
        }
    private:
        std::string _name;
};

// class testThreadPool2 {
//     public:
//         testThreadPool2(const std::string &name): _name(name) {

//         };
//         ~testThreadPool2() = default;

//         void testInt(int num) {
//             std::cout << std::this_thread::get_id() << " result is: " << num << std::endl;
//         }
//     private:
//         std::string _name;
// };

void testInt(int num)
{
    using namespace std::chrono_literals;
    // std::mutex mtx;
    std::ofstream tmp;
    // {
        // std::unique_lock<std::mutex>(mtx);
    tmp.open("tmp1nb" + std::to_string(num));
    // }

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
    tmp.close();
}


int main(void)
{
    // // using namespace std::chrono_literals;
    
    // // tcp::Server server(47920);


    // // if (!server.start())
    // //     exit(EXIT_FAILURE);
    // // // std::chrono::duration<float> mSeconds;
    thp::ThreadPool thpo(10);
    // thpo.init();
    // thpo.addNewTask(testInt, 10);
    // thpo.addNewTask(testInt, 20);
    // thpo.addNewTask(testInt, 30);
    // thpo.addNewTask(testInt, 40);
    // thpo.addNewTask(testInt, 50);
    // thpo.addNewTask(testInt, 60);
    // thpo.addNewTask(testInt, 70);
    // thpo.addNewTask(testInt, 80);
    // thpo.addNewTask(testInt, 90);
    // thpo.addNewTask(testInt, 100);
    // thpo.addNewTask(testInt, 110);
    // // std::chrono::steady_clock::time_point checkPoint = std::chrono::steady_clock::now();

    // while(1);
    // while (1)
    // {
    //     // mSeconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - checkPoint);
    //     // if (mSeconds.count() > 1)
    //     // {
    //     server.update();
    //         // checkPoint = std::chrono::steady_clock::now();
    //     // }
    // }

    tmr::myTimer timer;
    tmr::myTimer timer2;
    std::atomic_bool closeProgram = false;

    timer.startTimer(5, [&closeProgram]() {
        std::cout << "ca marche mon reuf" << std::endl;
    });

    timer2.startTimer(3, [&timer, &closeProgram]() {
        std::cout << "bah non on reset tout ca mon reuf" << std::endl;
        timer.resetTimer(5, [&closeProgram]{
            std::cout  << "Bah on a change mon reuf" << std::endl;
            closeProgram = true;
        });
    });

    while(!closeProgram);
}
