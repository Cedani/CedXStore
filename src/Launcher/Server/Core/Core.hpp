#pragma once
#include "Server.hpp"


namespace lau {
    class Core {
        public: 
            Core(int port);
            ~Core();


            void run();
        private:
            int _port;

    };
}