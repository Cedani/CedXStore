#include "ThreadPool.hpp"

thp::ThreadPool::ThreadPool(int size): _size(size), _isRunning(false), _cancelThread(false), _stopThread(false)
{
    // std::cout << "number of time" << std::endl;
}


thp::ThreadPool::~ThreadPool()
{
    if (_isRunning)
        cancel();
}

void thp::ThreadPool::run()
{
    while (true) {
        std::atomic<bool> popped = false;
        {
            std::function<void()> toLaunch;
            {
                std::unique_lock<std::shared_mutex> lock(_mutex);
                _cond.wait(lock, [this, &popped, &toLaunch](){
                    popped = _tasks.pop(toLaunch);
                    return _cancelThread.load() || _stopThread.load() || popped.load();
                });
            }
            // std::cout << std::this_thread::get_id() << std::endl;
            if (_cancelThread.load() || (_stopThread.load() && !popped)) {
                return;
            } else {
                toLaunch();
            }
        }
    }
}

void thp::ThreadPool::init()
{
    // _isRunning = true;
    // std::call_once(_once, [this](){
        // std::unique_lock<std::shared_mutex> lock(_mutex);
        std::shared_lock<std::shared_mutex> lock(_mutex);
        for (std::size_t i = 0; i < _size; ++i) {
            std::thread th1(&ThreadPool::run, this);
            std::cout << th1.get_id() << std::endl;
            _threads.push_back(std::move(th1));
        }
        _isRunning = true;
}

bool thp::ThreadPool::running() const
{
    std::shared_lock<std::shared_mutex> lock(_mutex);
    return _isRunning;
}

int thp::ThreadPool::length() const
{
    std::shared_lock<std::shared_mutex> lock(_mutex);
    return (_threads.size());
}

void thp::ThreadPool::cancel()
{
    {
        std::shared_lock<std::shared_mutex> lock(_mutex);
        if (!_isRunning.load())
            return;
        _cancelThread.store(true);
        _isRunning.store(false);
        _cond.notify_all();
        _tasks.clear();
    }
    for (auto &t: _threads)
        t.join();
}

void thp::ThreadPool::terminate()
{
    {
        std::shared_lock<std::shared_mutex> lock(_mutex);

        if (_cancelThread.load() || !_isRunning.load())
            return;
        _stopThread.store(true);
        _isRunning.store(false);
        _cond.notify_all();
    }
    for (auto &t: _threads) 
        t.join();
    std::cout << "Terminated" << std::endl;
}