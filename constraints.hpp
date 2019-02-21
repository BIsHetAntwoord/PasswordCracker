#ifndef CONSTRAINTS_HPP_INCLUDED
#define CONSTRAINTS_HPP_INCLUDED

#include <cstddef>
#include <string>
#include <vector>

class DictionaryElementPart;

class PasswordConstraints
{
    private:
        size_t min_len;
        size_t max_len;
        std::vector<std::string> required_classes;
    public:
        PasswordConstraints();
        ~PasswordConstraints();

        bool is_valid(const std::string&) const;
        bool should_continue(const std::string&, const std::vector<DictionaryElementPart>&, size_t) const;
        bool should_continue(const std::vector<DictionaryElementPart>&) const;
        bool is_possible(const std::vector<DictionaryElementPart>&) const;

        PasswordConstraints& min_length(size_t);
        PasswordConstraints& max_length(size_t);
        PasswordConstraints& require_one_of(const std::string&);
};

#endif // CONSTRAINTS_HPP_INCLUDED
