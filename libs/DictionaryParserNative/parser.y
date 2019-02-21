%{
#include "parsedata.hpp"
#include "parser.hpp"

#define YYLEX_PARAM x->lexer
%}

%error-verbose
%pure-parser
%parse-param {parse_info* x}
%lex-param {void* lexer}

%token COMMAND_NAME COMMAND_DATE COMMAND_WORD
%token OPEN_PAR CLOSE_PAR COMMA
%token<str> STRING
%token<integer> INTEGER
%token UNKNOWN_TOKEN

%type<str_list> string_list

%destructor {delete $$;} <str> <str_list>

%%

root
    : command_list
    ;

command_list
    : command_list command
    |
    ;

command
    : name_command
    | date_command
    | word_command
    ;

name_command
    : COMMAND_NAME OPEN_PAR STRING CLOSE_PAR                                                {x->parser->add_name(x, *$3); delete $3;}
    ;

date_command
    : COMMAND_DATE OPEN_PAR INTEGER COMMA INTEGER COMMA INTEGER CLOSE_PAR                   {x->parser->add_date(x, $3, $5, $7);}
    ;

word_command
    : COMMAND_WORD OPEN_PAR string_list CLOSE_PAR                                           {x->parser->add_word(x, *$3); delete $3;}
    ;

string_list
    : string_list COMMA STRING                                                              {$$ = $1; $$->push_back(*$3); delete $3;}
    | STRING                                                                                {$$ = new std::vector<std::string>(); $$->push_back(*$1); delete $1;}
    ;

%%
