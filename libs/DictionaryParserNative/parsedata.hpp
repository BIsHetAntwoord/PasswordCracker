#ifndef PARSEDATA_HPP_INCLUDED
#define PARSEDATA_HPP_INCLUDED

#include <cstddef>
#include <cstdio>
#include <string>
#include <vector>
#include <cstdint>

#include <lua.h>

class Parser;

union YYSTYPE
{
    std::string* str;
    size_t integer;
    std::vector<std::string>* str_list;
};

#define YYSTYPE_IS_DECLARED 1

struct parse_info
{
    void* lexer;

    size_t errors;
    std::string error_msg;
    size_t lines;

    lua_State* state;
    Parser* parser;
};

int yylex_init(void**);
int yylex_destroy(void*);
void yyset_extra(parse_info*, void*);
void yyset_in(FILE*, void*);
int yylex(YYSTYPE*, void*);
int yyparse(parse_info*);
void yyerror(parse_info*, const char*, ...);

#endif // PARSEDATA_HPP_INCLUDED
