#pragma once
#include "ThreadPool.hpp"

namespace lau {
    class Manager {
        static Manager& getInstance();

        thp::ThreadPool &getThreadpool();
        private:
            Manager();
            ~Manager() = default;

            Manager(const Manager &) = delete;
            Manager &operator=(const Manager &) = delete;
            thp::ThreadPool _threadPool;
    };
}