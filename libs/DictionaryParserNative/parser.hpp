#ifndef PARSER_HPP_INCLUDED
#define PARSER_HPP_INCLUDED

#include <cstdio>
#include <cstddef>
#include <string>
#include <vector>

#include <lua.h>

typedef void(*name_callback_type)(lua_State*, void*, const std::string&);
typedef void(*date_callback_type)(lua_State*, void*, size_t, size_t, size_t);
typedef void(*word_callback_type)(lua_State*, void*, const std::vector<std::string>&);

struct data_callbacks
{
    void* data;
    name_callback_type name_callback;
    date_callback_type date_callback;
    word_callback_type word_callback;
};

struct parse_info;

class Parser
{
    private:
        data_callbacks callbacks;
    public:
        Parser(const data_callbacks&);
        ~Parser();

        bool parse(lua_State*, FILE* stream, std::string&);
        data_callbacks& get_callbacks();

        void add_name(parse_info*, const std::string&);
        void add_date(parse_info*, size_t, size_t, size_t);
        void add_word(parse_info*, const std::vector<std::string>&);
};

#endif // PARSER_HPP_INCLUDED
