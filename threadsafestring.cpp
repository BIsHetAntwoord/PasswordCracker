#include "threadsafestring.hpp"

#include <boost/thread/lock_guard.hpp>

ThreadSafeString::ThreadSafeString() {}
ThreadSafeString::ThreadSafeString(const ThreadSafeString& tss) : str(tss.str) {}
ThreadSafeString::~ThreadSafeString() {}

ThreadSafeString& ThreadSafeString::operator=(const std::string& str)
{
    boost::lock_guard<boost::mutex> lock(this->mutex);
    this->str = str;
    return *this;
}

ThreadSafeString& ThreadSafeString::operator=(const ThreadSafeString& str)
{
    boost::lock_guard<boost::mutex> lock(this->mutex);
    this->str = str.str;
    return *this;
}

std::string ThreadSafeString::to_stdstr()
{
    boost::lock_guard<boost::mutex> lock(this->mutex);
    std::string result = this->str;

    return std::move(result);
}
