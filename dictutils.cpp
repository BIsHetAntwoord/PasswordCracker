#include "dictutils.hpp"

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/case_conv.hpp>

std::vector<std::string> split_words(const std::string& words)
{
    std::vector<std::string> result;
    boost::split(result, words, boost::is_any_of(" -,."), boost::token_compress_on);
    return result;
}

std::vector<std::string> split_syllables(const std::string& word)
{
    std::vector<std::string> result;

    result.push_back(word); //TODO, actual splitting

    return result;
}

std::string uppercase(const std::string& str)
{
    return boost::to_upper_copy<std::string>(str);
}

std::string lowercase(const std::string& str)
{
    return boost::to_lower_copy<std::string>(str);
}

bool islowercase(const std::string& str)
{
    return std::all_of(str.begin(), str.end(), [](int c){return isalpha(c) ? islower(c) : true;});
}

bool isuppercase(const std::string& str)
{
    return std::all_of(str.begin(), str.end(), [](int c){return isalpha(c) ? isupper(c) : true;});
}
