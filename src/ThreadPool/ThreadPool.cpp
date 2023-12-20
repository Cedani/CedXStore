#include "ThreadPool.hpp"

thp::ThreadPool::ThreadPool(int size): _size(size), _isRunning(false), _cancelThread(false), _stopThread(false), _pauseThread(false)
{
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
        std::function<void()> toLaunch;
        {
            std::unique_lock<std::shared_mutex> lock(_mutex);
            _cond.wait(lock, [this, &popped, &toLaunch](){
                if (!_pauseThread)
                    popped.store(_tasks.pop(toLaunch));
                return _cancelThread.load() || _stopThread.load() || popped.load();
            });
        }
        if (_cancelThread.load() || (_stopThread.load() && !popped.load())) {
            return;
        } else {
            // {
            //     std::unique_lock lock(_mutex);
            //     _cond.notify_all();
            // }
            toLaunch();
            // std::cout << "finished executing" << std::endl;
            // if (_tasks.size() == 0) {
            //     std::unique_lock lock(_mutex);
            //     _cond.notify_all();
            // }
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
        // std::cout << th1.get_id() << std::endl;
        _threads.push_back(std::move(th1));
    }
    _isRunning.store(true);
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
    if (!_isRunning.load())
        return;
    _cancelThread.store(true);
    _isRunning.store(false);
    {
        std::shared_lock<std::shared_mutex> lock(_mutex);
        _cond.notify_all();
        _tasks.clear();
    }
    for (std::thread &t: _threads)
        t.join();
}

void thp::ThreadPool::terminate()
{
    if (_cancelThread.load() || !_isRunning.load())
        return;
    _stopThread.store(true);
    _isRunning.store(false);
    {   
        std::shared_lock<std::shared_mutex> lock(_mutex);
        _cond.notify_all();
    }
    for (auto &t: _threads) 
        t.join();
}

void thp::ThreadPool::restart(int size)
{
    if (_isRunning)
        terminate();

    _threads.clear();
    _cancelThread = false;
    _stopThread = false;
    _size = size;
    init();
}

void thp::ThreadPool::pause()
{
    if (!_isRunning || _cancelThread || _stopThread || _pauseThread)
        return;
    _pauseThread = true;
    {
        std::shared_lock<std::shared_mutex> lock(_mutex);
        _cond.notify_all();
    }
    // std::cout << "paused" << std::endl;
}

void thp::ThreadPool::resume()
{
    if (!_pauseThread)
        return;
    _pauseThread = false;
    {
        std::shared_lock<std::shared_mutex> lock(_mutex);
        _cond.notify_all();
    }
    // std::cout << "resumed" << std::endl;
}

// void thp::ThreadPool::waitUntilFinished()
// {
//     {
//         std::unique_lock lock(_mutex);
//         _cond.wait(lock, [this](){
//             return (_nbWorkers == 0);
//         });
//     }
//     std::cout << "workers has finished" << std::endl;
// }