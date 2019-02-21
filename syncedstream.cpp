#include "syncedstream.hpp"

#include <iostream>

SynchronizedStream sync_cout(std::cout);
SynchronizedStream sync_cerr(std::cerr);

LockedStream::LockedStream(std::ostream& os, boost::mutex& mutex) : os(os), lock(mutex) {}
LockedStream::LockedStream(LockedStream&& stream) : os(stream.os), lock(std::move(stream.lock)) {}
LockedStream::~LockedStream() {}

SynchronizedStream::SynchronizedStream(std::ostream& os) : os(os) {}
SynchronizedStream::~SynchronizedStream() {}

LockedStream operator<<(SynchronizedStream& os, std::ostream&(*manip)(std::ostream&))
{
    LockedStream stream(os.os, os.mutex);
    stream << manip;
    return stream;
}

LockedStream& operator<<(LockedStream& os, std::ostream&(*manip)(std::ostream&))
{
    os.os << manip;
    return os;
}

LockedStream&& operator<<(LockedStream&& os, std::ostream&(*manip)(std::ostream&))
{
    os.os << manip;
    return std::move(os);
}
