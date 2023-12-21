#include "Manager.hpp"

lau::Manager::Manager(): _threadPool(15)
{

}

lau::Manager &lau::Manager::getInstance()
{
    static Manager manager;
    return manager;
}

thp::ThreadPool &lau::Manager::getThreadpool()
{
    return _threadPool;
}
