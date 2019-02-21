#ifndef THREADSAFESTRING_HPP_INCLUDED
#define THREADSAFESTRING_HPP_INCLUDED

#include <boost/thread/mutex.hpp>

class ThreadSafeString
{
    private:
        boost::mutex mutex;
        std::string str;
    public:
        ThreadSafeString();
        ThreadSafeString(const ThreadSafeString&);
        ~ThreadSafeString();

        ThreadSafeString& operator=(const ThreadSafeString&);
        ThreadSafeString& operator=(const std::string&);

        std::string to_stdstr();
};

#endif // THREADSAFESTRING_HPP_INCLUDED
