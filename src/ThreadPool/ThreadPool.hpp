#pragma once

#include "ThreadSafeQueue.hpp"
#include <thread>
#include <vector>
#include <condition_variable>
#include <functional>
#include <memory>
#include <atomic>
namespace thp {
    class ThreadPool {
        public:
            ThreadPool(int size);
            ~ThreadPool();

            void init();

            void run();

            // void add();
            
            // void remove();
            

            template<class T, class... Args>
            void addNewTask(T &&func, Args&&... args){
                {
                    std::shared_lock<std::shared_mutex> lock(_mutex);
                    if (!_isRunning || _pauseThread) {
                        std::cout << "[THREADPOOL]: no task added due to pause or cancel of the threadpool." << std::endl;
                        return;
                    }
                }
                auto toLaunch = std::bind(std::forward<T>(func), std::forward<Args>(args)...);
                _tasks.emplace_back(toLaunch);
                _cond.notify_one();
            };

            template<class T, class C, class...Args>
            void addClassTask(T &&func, C *obj, Args&&... args) {
                {
                    std::shared_lock<std::shared_mutex> lock(_mutex);
                    if (!_isRunning) {
                        std::cout << "[THREADPOOL]: no task added due to pause or cancel of the threadpool." << std::endl;
                        return;
                    }
                }
                auto toLaunch = std::bind(std::forward<T>(func), std::ref(*obj), std::forward<Args>(args)...);
                _tasks.emplace_back(toLaunch);
                _cond.notify_one();
            };

            template<class C, class...Args>
            void addNewTask(std::function<void()> func)
            {
                {
                    std::shared_lock<std::shared_mutex> lock(_mutex);
                    if (!_isRunning) {
                        std::cout << "[THREADPOOL]: no task added due to pause or cancel of the threadpool." << std::endl;
                        return;
                    }
                }
                _tasks.emplace_back(func);
                _cond.notify_one();
            }

            bool running() const;
            int length() const;
            void cancel();
            void terminate();
            void restart(int size);
            void pause();
            void resume();
            void waitUntilFinished();

        private:
            std::atomic<int> _size;
            std::atomic<int> _nbWorkers;
            std::atomic<bool> _isRunning;
            std::atomic<bool> _cancelThread;
            std::atomic<bool> _stopThread;
            std::atomic<bool> _pauseThread;
            std::condition_variable_any _cond;
            ThreadSafeQueue<std::function<void()>> _tasks;
            mutable std::shared_mutex _mutex;
            std::vector<std::thread> _threads;
            std::once_flag _once;
    };
}