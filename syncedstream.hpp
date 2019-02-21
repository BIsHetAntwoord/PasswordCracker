#ifndef SYNCEDSTREAM_HPP_INCLUDED
#define SYNCEDSTREAM_HPP_INCLUDED

#include <iosfwd>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/lock_types.hpp>

class LockedStream
{
    private:
        std::ostream& os;
        boost::mutex::scoped_lock lock;
    public:
        LockedStream(std::ostream&, boost::mutex& mutex);
        LockedStream(const LockedStream&) = delete;
        LockedStream(LockedStream&&);
        ~LockedStream();

        LockedStream& operator=(const LockedStream&) = delete;

        template <typename T>
        friend LockedStream& operator<<(LockedStream&, const T&);
        friend LockedStream& operator<<(LockedStream&, std::ostream&(*)(std::ostream&));
        template <typename T>
        friend LockedStream&& operator<<(LockedStream&&, const T&);
        friend LockedStream&& operator<<(LockedStream&&, std::ostream&(*)(std::ostream&));
};

class SynchronizedStream
{
    private:
        std::ostream& os;
        boost::mutex mutex;
    public:
        SynchronizedStream(std::ostream&);
        SynchronizedStream(const SynchronizedStream&) = delete;
        ~SynchronizedStream();

        SynchronizedStream& operator=(const SynchronizedStream&) = delete;

        template <typename T>
        friend LockedStream operator<<(SynchronizedStream&, const T&);
        friend LockedStream operator<<(SynchronizedStream&, std::ostream&(*)(std::ostream&));
};

//Output stream operators
template <typename T>
LockedStream operator<<(SynchronizedStream&, const T&);
LockedStream operator<<(SynchronizedStream&, std::ostream&(*)(std::ostream&));
template <typename T>
LockedStream& operator<<(LockedStream&, const T&);
LockedStream& operator<<(LockedStream&, std::ostream&(*)(std::ostream&));
template <typename T>
LockedStream&& operator<<(LockedStream&&, const T&);
LockedStream&& operator<<(LockedStream&&, std::ostream&(*)(std::ostream&));

extern SynchronizedStream sync_cout;
extern SynchronizedStream sync_cerr;

#include "syncedstream.inl"

#endif // SYNCEDSTREAM_HPP_INCLUDED
