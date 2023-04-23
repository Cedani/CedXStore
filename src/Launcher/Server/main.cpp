#include "Server.hpp"

using nlohmann::json;


class tester{
    public:
        tester()
        {

        }

        ~tester()
        {

        }

        void testConnection(json q, tcp::Connection &cli)
        {
            json response = {
                {"message", "ouais c'est bon"},
                {"message2", "ca va pas ou quoi, niafouroudey"}
            };
            cli.writeMessage(response.dump());
        }
};

void testConnection(json q, tcp::Connection &cli)
{
    json response = {
        {"message", "ouais c'est bon de francis"},
        {"message2", "ca va pas ou quoi, ton maudia"}
    };
    cli.writeMessage(response.dump());
}

int main(void)
{
    tcp::Server server(47920);

    tester test;
    server.addRoute("test1", [](json &j, tcp::Connection &cli) {
        testConnection(j, cli);
    });

    server.addRoute("test2", [test](json &j, tcp::Connection &cli) mutable -> void {
        test.testConnection(j, cli);
    });

    if (!server.start())
        exit(EXIT_FAILURE);
    while (1) {
        server.update();
    }
}