%{
#include "parsedata.hpp"
#include "parser.parser.hh"
%}

%option noyywrap
%option nounput
%option never-interactive
%option bison-bridge
%option reentrant
%option extra-type="struct parse_info*"

DIGIT       [0-9]
WSPACE      [ \t\f\v\b]
NEWLINE     \r?\n|\r

%x STR

%%

<INITIAL>
{
    "@NAME"                                                     {return COMMAND_NAME;}
    "@DATE"                                                     {return COMMAND_DATE;}
    "@WORD"                                                     {return COMMAND_WORD;}

    "("                                                         {return OPEN_PAR;}
    ")"                                                         {return CLOSE_PAR;}
    ","                                                         {return COMMA;}

    {DIGIT}+                                                    {
                                                                    errno = 0;
                                                                    size_t result = std::strtoull(yytext, NULL, 10);
                                                                    if(errno == ERANGE)
                                                                        yyerror(yyextra, "Integer out of range: %s", yytext);
                                                                    yylval->integer = result;
                                                                    return INTEGER;
                                                                }
    \"                                                          {yylval->str = new std::string(); BEGIN(STR);}

    {WSPACE}                                                    {}
    {NEWLINE}                                                   {++yyextra->lines;}
    .                                                           {return UNKNOWN_TOKEN;}
}

<STR>
{
    "\\\\"                                                      {yylval->str->append("\\");}
    "\\\""                                                      {yylval->str->append("\"");}
    \"                                                          {BEGIN(INITIAL); return STRING;}
    .                                                           {yylval->str->append(yytext);}
}

%%
