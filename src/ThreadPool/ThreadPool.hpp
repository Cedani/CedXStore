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


            template<class T, class... Args>
            void addNewTask(T&&func, Args&&... args){
                {
                    std::shared_lock<std::shared_mutex> lock(_mutex);
                    if (!_isRunning) {
                        std::cout << "[ARCADE THREADPOOL]: no task added due to pause or cancel of the threadpool." << std::endl;
                        return;
                    }
                }
                auto toLaunch = std::bind(&func, std::forward<Args>(args)...);
                _tasks.emplace_back([&toLaunch, &args...](){
                    toLaunch();
                });
                _cond.notify_one();
                std::cout << "bind" << std::endl;
            };

            template<class T, class C, class...Args>
            void addClassTask(T &&func, C &obj, Args&&... args) {
                {
                    std::shared_lock<std::shared_mutex> lock(_mutex);
                    if (!_isRunning) {
                        std::cout << "[ARCADE THREADPOOL]: no task added due to pause or cancel of the threadpool." << std::endl;
                        return;
                    }
                }
                auto toLaunch = std::bind(func, obj, std::forward<Args>(args)...);
                _tasks.emplace_back([&toLaunch, &args...](){
                    toLaunch();
                });
                _cond.notify_one();
                std::cout << "bind" << std::endl;
            };

            bool running() const;
            int length() const;
            void cancel();
            void terminate();

        private:
            int _size;
            std::atomic<bool> _isRunning;
            std::atomic<bool> _cancelThread;
            std::atomic<bool> _stopThread;
            std::condition_variable_any _cond;
            ThreadSafeQueue<std::function<void()>> _tasks;
            mutable std::shared_mutex _mutex;
            std::vector<std::thread> _threads;
            std::once_flag _once;
            // std::atomic_bool _done;
            
    };

    // template<class T, class... Args>
    // void ThreadPool::addNewTask(T&& func, Args&& ...args) 

    // extern "C" std::unique_ptr<ThreadPool> createThreadPool(int size);
}