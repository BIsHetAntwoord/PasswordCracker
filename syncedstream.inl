#include <iostream>

template <typename T>
LockedStream operator<<(SynchronizedStream& os, const T& arg)
{
    LockedStream stream(os.os, os.mutex);
    stream << arg;
    return stream;
}

template <typename T>
LockedStream& operator<<(LockedStream& os, const T& arg)
{
    os.os << arg;
    return os;
}

template <typename T>
LockedStream&& operator<<(LockedStream&& os, const T& arg)
{
    os.os << arg;
    return std::move(os);
}
