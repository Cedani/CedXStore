#ifdef _WIN32
#include <iostream>
#include "App/App.hpp"
#include "Client.hpp"

using nlohmann::json;

int main(int argc, char **argv)
{
    // lau::Client myCLi;

    // if (!myCLi.connectToServer())
    //     return 84;

    // std::cout << myCLi.readMessage() << std::endl;

    // // for (int i = 1; i > 0; ++i) {
    //     myCLi.writeMessage(json{
    //         {"command", "pseudoAvailability"},
    //         {"data", 
    //             {
    //                 {"pseudo", {"testeurdu34"}}
    //             }
    //         }
    //     }.dump() + "\r\n");
    //     std::cout << myCLi.readMessage() << std::endl;
    // // }
    lau::App *_myapp = new lau::App;
    return (wxEntry(argc, argv));
}
#endif