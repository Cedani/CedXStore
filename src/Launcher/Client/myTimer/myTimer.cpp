#include "myTimer.hpp"

tmr::myTimer::myTimer(): _stop(false), _delay(-1)
{
    _runner = std::thread(&myTimer::run, this);
}

tmr::myTimer::~myTimer()
{
    _stop = true;
    _cVar.notify_one();
    if (_runner.joinable())
        _runner.join();
}

void tmr::myTimer::resetTimer()
{
    _clock = std::chrono::steady_clock::now();
    _delay = -1.0f;
    _funcToLaunch = std::function<void()> {};
    _cVar.notify_one();
}

void tmr::myTimer::resetTimer(float time, std::function<void()> func)
{
    _clock = std::chrono::steady_clock::now();
    _delay = time;
    _funcToLaunch = func;
    _cVar.notify_one();
}

void tmr::myTimer::startTimer(float delay, std::function<void()> func)
{
    _delay = delay;
    _clock = std::chrono::steady_clock::now();
    _funcToLaunch = func;
    _cVar.notify_one();
}

void tmr::myTimer::run()
{
    while (!_stop) {
        {
            std::unique_lock<std::mutex> lock(_mtx);
            _cVar.wait(lock, [this]() {
                return (_delay != -1.0f || _stop);
            });
        }
        if (_stop) break;
        std::chrono::duration<float> checkPoint;

        checkPoint = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - _clock);
        // std::cout << "checkpoint: " << checkPoint.count() << " seconds" << std::endl;
        if (_delay > 0.f && checkPoint.count() >= _delay) {
            std::cout << "executing function after delay" << std::endl;
            _funcToLaunch();
            _delay = -1.0f;
        }
    }
}

void tmr::myTimer::stopTimer()
{
    _stop = true;
    _cVar.notify_one();
}