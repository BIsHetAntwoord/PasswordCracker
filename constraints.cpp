#include "constraints.hpp"
#include "dictionary.hpp"

PasswordConstraints::PasswordConstraints() : min_len(0), max_len(256) {}
PasswordConstraints::~PasswordConstraints() {}

bool PasswordConstraints::is_valid(const std::string& str) const
{
    size_t length = str.length();
    if(length < this->min_len || length > this->max_len)
        return false;

    //Check if all classes are contained
    for(const std::string& cls : this->required_classes)
    {
        bool continue_inner = true;
        for(size_t i = 0; i < cls.size() && continue_inner; ++i)
        {
            if(str.find(cls[i]) != std::string::npos)
                continue_inner = false;
        }
        if(continue_inner)
            return false;
    }
    return true;
}

bool PasswordConstraints::should_continue(const std::string& str, const std::vector<DictionaryElementPart>& parts, size_t index) const
{
    size_t min = str.length();
    for(size_t i = index; i < parts.size(); ++i)
    {
        const DictionaryElementPart& part = parts[i];
        size_t part_min = 0;
        for(size_t i = 0; i < part.alias_count(); ++i)
        {
            size_t length = part.alias(i).length();
            if(part_min == 0 || part_min > length)
                part_min = length;
        }
        min += part_min;
    }

    return min < this->max_len;
}

bool PasswordConstraints::should_continue(const std::vector<DictionaryElementPart>& parts) const
{
    return this->should_continue("", parts, 0);
}

bool PasswordConstraints::is_possible(const std::vector<DictionaryElementPart>& parts) const
{
    //Check if all classes are contained
    for(const std::string& cls : this->required_classes)
    {
        bool continue_inner = true;
        for(size_t i = 0; i < parts.size() && continue_inner; ++i)
        {
            for(size_t j = 0; j < parts[i].alias_count() && continue_inner; ++j)
            {
                for(size_t k = 0; k < cls.size() && continue_inner; ++k)
                {
                    if(parts[i].alias(j).find(cls[k]) != std::string::npos)
                        continue_inner = false;
                }
            }
        }
        if(continue_inner) //Not found
            return false;
    }

    //Check the sizes
    size_t min = 0;
    size_t max = 0;
    for(const DictionaryElementPart& part : parts)
    {
        size_t part_min = 0;
        size_t part_max = 0;
        for(size_t i = 0; i < part.alias_count(); ++i)
        {
            size_t length = part.alias(i).length();
            if(part_max < length)
                part_max = length;
            if(part_min == 0 || part_min > length)
                part_min = length;
        }
        min += part_min;
        max += part_max;
    }

    return !(max < this->min_len || min > this->max_len);
}

PasswordConstraints& PasswordConstraints::min_length(size_t min)
{
    this->min_len = min;
    return *this;
}

PasswordConstraints& PasswordConstraints::max_length(size_t max)
{
    this->max_len = max;
    return *this;
}

PasswordConstraints& PasswordConstraints::require_one_of(const std::string& str)
{
    this->required_classes.push_back(str);
    return *this;
}
