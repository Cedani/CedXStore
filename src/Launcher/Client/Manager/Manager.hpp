#pragma once
#include <iostream>
#include <thread>
#include <nlohmann/json.hpp>
#include "Client.hpp"
#include "myUtils.hpp"

namespace lau {
    class Manager {
        public:
            Manager();
            ~Manager();

            bool initClientLogin();

            //check if pseudo is availble
            nlohmann::json checkPseudoAvailability(const char *);
            // void initMainClient();



        // methods
        private:

        // members
        private:
            // Principal client and thread
            // std::thread mainClientThread;
            Client _mainClient;
            // thp::ThreadPool _threadPool;

    };
}