#include "parser.hpp"
#include "parsedata.hpp"

#include <iostream>
#include <sstream>
#include <cstring>

void yyerror(parse_info* x, const char* msg, ...)
{
    int size = std::strlen(msg) * 2;
    std::string str;
    va_list arg_list;

    while(true)
    {
        str.resize(size);
        va_start(arg_list, msg);

        int required_size = vsnprintf(&str[0], size, msg, arg_list);

        va_end(arg_list);
        if(required_size >= 0 && required_size < size)
        {
            //Success
            str.resize(required_size);

            std::stringstream ss;
            if(x->errors > 0)
                ss << std::endl;
            ss << "Error at line " << x->lines << ": " << str;

            x->error_msg += ss.str();
            ++x->errors;
            return;
        }
        else if(required_size >= 0)
            size = required_size + 1;
        else
            size *= 2;
    }
}

Parser::Parser(const data_callbacks& callbacks) : callbacks(callbacks) {}

Parser::~Parser()
{
    std::cout << "Parser destruction at " << this << std::endl;
}

bool Parser::parse(lua_State* L, FILE* stream, std::string& errors)
{
    parse_info parser;
    parser.state = L;
    parser.errors = 0;
    parser.parser = this;
    parser.lines = 1;

    yylex_init(&parser.lexer);
    yyset_in(stream, parser.lexer);
    yyset_extra(&parser, parser.lexer);
    yyparse(&parser);
    yylex_destroy(parser.lexer);

    errors = parser.error_msg;
    return parser.errors == 0;
}

data_callbacks& Parser::get_callbacks()
{
    return this->callbacks;
}

void Parser::add_name(parse_info* x, const std::string& name)
{
    this->callbacks.name_callback(x->state, this->callbacks.data, name);
}

void Parser::add_date(parse_info* x, size_t day, size_t month, size_t year)
{
    this->callbacks.date_callback(x->state, this->callbacks.data, day, month, year);
}

void Parser::add_word(parse_info* x, const std::vector<std::string>& word)
{
    this->callbacks.word_callback(x->state, this->callbacks.data, word);
}
