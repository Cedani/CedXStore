#pragma once
#include <iostream>
#include <chrono>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace tmr{
    class myTimer {
        public:
            myTimer();
            ~myTimer();

            void resetTimer();
            void resetTimer(float time, std::function<void()>);
            void startTimer(float, std::function<void()>);
            void run();
            void stopTimer();

        private:
            std::atomic<float> _delay;
            std::chrono::steady_clock::time_point _clock;
            std::thread _runner;
            bool _stop;
            std::function<void()> _funcToLaunch;
            std::mutex _mtx;
            std::condition_variable _cVar;
    };
}