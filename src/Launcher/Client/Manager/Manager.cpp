#include "Manager.hpp"

using nlohmann::json;

lau::Manager::Manager()
{

}

lau::Manager::~Manager()
{
    
}

void lau::Manager::initClientLogin()
{
    if (!_mainClient.connectToServer()) {
        std::cout << "Cant connect to server" << std::endl;
        exit(EXIT_FAILURE); 
    }

    std::cout << _mainClient.readMessage() << std::endl;
}

json lau::Manager::checkPseudoAvailability(const char *pseudo)
{
    json result = json{
        {"command", "pseudoAvailability"},
        {"data", {
            {"pseudo", pseudo}
        }}
    };

    _mainClient.writeMessage(result.dump());
    return _mainClient.readMessage();
}