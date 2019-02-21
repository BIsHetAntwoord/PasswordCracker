#include <algorithm>

template <typename It, typename T>
bool has_sorted_element(It begin, It end, T value)
{
    It i = std::lower_bound(begin, end, value);

    return i != end && *i == value;
}
