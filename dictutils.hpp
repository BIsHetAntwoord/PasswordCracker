#ifndef DICTUTILS_HPP_INCLUDED
#define DICTUTILS_HPP_INCLUDED

#include <vector>
#include <string>

std::vector<std::string> split_words(const std::string&);
std::vector<std::string> split_syllables(const std::string&);

std::string uppercase(const std::string&);
std::string lowercase(const std::string&);

bool islowercase(const std::string&);
bool isuppercase(const std::string&);

#endif // DICTUTILS_HPP_INCLUDED
