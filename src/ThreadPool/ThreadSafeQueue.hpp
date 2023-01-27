#pragma once

#include <iostream>
#include <queue>
#include <mutex>
#include <shared_mutex>

namespace thp {
    template<typename T>
    class ThreadSafeQueue {
        public:
            ThreadSafeQueue() {

            };

            ~ThreadSafeQueue(){
                clear();
            }

            bool empty() {
                std::shared_lock<std::shared_mutex> lock(_mtx);
                std::cout << "je rentre ici" << std::endl;
                return _queue.empty();
            }

            std::size_t size() {
                std::shared_lock<std::shared_mutex> lock(_mtx);
                return _queue.size();
            }

            template<typename ...Args>
            void emplace_back(Args&&... args){
                std::unique_lock<std::shared_mutex> lock(_mtx);
                _queue.emplace(std::forward<Args>(args)...);
            }


            bool pop(T &dest) {
                std::unique_lock<std::shared_mutex> lock(_mtx);
                if (_queue.empty())
                    return false;
                dest = std::move(_queue.front());
                _queue.pop();
                return true;
            }

            void clear() {
                std::unique_lock<std::shared_mutex> lock(_mtx);
                for (int i = _queue.size(); i > 0; --i) {
                    _queue.pop();
                }
            }

        private:
            std::shared_mutex _mtx;
            std::queue<T> _queue;
    };
}