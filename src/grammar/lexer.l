    
    /*
    TODO: 
    - Check this case: 123aaa is matched as 123:INTEGER aaa:IDENTIFIER
    */

    /*
    Comments can start with // or /*
    But MUST be indented, or have at least one space before the comment.
    */

    /*
    Options Doumentation: https://ftp.gnu.org/old-gnu/Manuals/flex-2.5.4/html_node/flex_17.html
    */

    /*
    Makes the generated scanner run in debug mode. Whenever a pattern is recognized and the global yy_flex_debug is non-zero (which is the default), the scanner will write to stderr a line of the form:
    --accepting rule at line 53 ("the matched text")
    The line number refers to the location of the rule in the file defining the scanner (i.e., the file that was fed to flex).
    Messages are also generated when the scanner backs up, accepts the default rule, reaches the end of its input buffer (or encounters a NUL; at this point, the two look the same as far as the scanner's concerned), or reaches an end-of-file.
    */
%option debug

    /*
    Causes the default rule (that unmatched scanner input is echoed to stdout) to be suppressed.
    If the scanner encounters input that does not match any of its rules, it aborts with an error.
    This option is useful for finding holes in a scanner's rule set.
    */
%option nodefault

    /*
    Makes the scanner not call `yywrap()' upon an end-of-file,
    but simply assume that there are no more files to scan
    (until the user points yyin at a new file and calls `yylex()' again)
    */
%option noyywrap


%option warn

    /*
    Creates global variable `yylineno` which contains the current line read from input
    */
%option yylineno

%{
#include <iostream>
#include <string>

#include "parser.hpp"

#undef YY_DECL
// Give Flex the prototype of yylex we want which is compatible with Bison C++ variants
#define YY_DECL yy::parser::symbol_type yylex()

%}


DIGIT               [0-9]
INTEGER             [0-9]+
FLOAT               (([0-9]*\.[0-9]+)|([0-9]+\.[0-9]*))
EXP                 ([eE][-+]?{INTEGER})
REAL                ({INTEGER}{EXP}|{FLOAT}{EXP}?)
LETTER              [a-zA-Z_]
CHAR                \'.\'
STRING              \".*\"
IDENTIFIER          {LETTER}({LETTER}|{DIGIT})*
LINE_COMMENT        "//"(.)*
MULTILINE_COMMENT   [/][*]+([^*/][^*]*[*]+)+[/]


%%

"int"                   {printf("[LEXER] Data Type keyword: int\n");}
"float"                 {printf("[LEXER] Data Type keyword: float\n");}
"char"                  {printf("[LEXER] Data Type keyword: char\n");}
"bool"                  {printf("[LEXER] Data Type keyword: bool\n");}
"void"                  {printf("[LEXER] Data Type keyword: void\n");}
"string"                {printf("[LEXER] Data Type keyword: string\n");}

"const"                 {printf("[LEXER] reserved keyword: const\n");}
"if"                    {printf("[LEXER] reserved keyword: if\n");}
"else"                  {printf("[LEXER] reserved keyword: else\n");}
"switch"                {printf("[LEXER] reserved keyword: switch\n");}
"case"                  {printf("[LEXER] reserved keyword: case\n");}
"default"               {printf("[LEXER] reserved keyword: default\n");}
"for"                   {printf("[LEXER] reserved keyword: for\n");}
"do"                    {printf("[LEXER] reserved keyword: do\n");}
"while"                 {printf("[LEXER] reserved keyword: while\n");}
"break"                 {printf("[LEXER] reserved keyword: break\n");}
"continue"              {printf("[LEXER] reserved keyword: continue\n");}
"return"                {printf("[LEXER] reserved keyword: return\n");}


"++"                    {printf("[LEXER] operator INC: ++\n");}
"--"                    {printf("[LEXER] operator DEC: --\n");}
"=="                    {printf("[LEXER] operator EQUAL: ==\n");}
"!="                    {printf("[LEXER] operator NOT_EQUAL: !=\n");}
">="                    {printf("[LEXER] operator GREATER_EQUAL: >=\n");}
"<="                    {printf("[LEXER] operator LESS_EQUAL: <=\n");}
"<<"                    {printf("[LEXER] operator SHIFT_LEFT: <<\n");}
">>"                    {printf("[LEXER] operator SHIFT_RIGHT: >>\n");}
"&&"|"and"              {printf("[LEXER] operator AND:: %s\n", yytext);}
"||"|"or"               {printf("[LEXER] operator OR:: %s\n", yytext);}
"!"|"not"               {printf("[LEXER] operator NOT: %s\n", yytext);}
"**"                    {printf("[LEXER] operator POWER: **\n");}

"+"                     {printf("[LEXER] operator PLUS: +\n"); return yy::parser::make_PLUS();}
"-"                     {printf("[LEXER] operator MINUS: -\n"); return yy::parser::make_MINUS();}
"*"                     {printf("[LEXER] operator MULTIPLY: *\n"); return yy::parser::make_MULTIPLY();}
"/"                     {printf("[LEXER] operator DIVISON: /\n"); return yy::parser::make_DIVISION();}
"="                     {printf("[LEXER] operator ASSIGN: =\n"); return yy::parser::make_ASSIGN();}
";"                     {printf("[LEXER] operator SEMICOLON: ;\n"); return yy::parser::make_SEMICOLON();}

[<>(){}[\],:]           {printf("[LEXER] ANOTHER operator: %s\n", yytext);}


{INTEGER}               {printf("[LEXER] INTEGER: %s\n", yytext); return yy::parser::make_INTEGER(atoi(yytext));}
{REAL}                  {printf("[LEXER] REAL: %s\n", yytext);}
{CHAR}                  {printf("[LEXER] CHAR: %s\n", yytext);}
{STRING}                {printf("[LEXER] STRING: %s\n", yytext);}
"true"                  {printf("[LEXER] BOOLEAN TRUE: %s\n", yytext);}
"false"                 {printf("[LEXER] BOOLEAN FALSE: %s\n", yytext);}
{IDENTIFIER}            {printf("[LEXER] IDENTIFIER: %s\n", yytext);return yy::parser::make_IDENTIFIER(yytext);}
{LINE_COMMENT}          {printf("[LEXER] SINGLE LINE COMMENT: %s\n", yytext);}
{MULTILINE_COMMENT}     {printf("[LEXER] MULTI_LINE COMMENT: %s\n", yytext);}
[ |\n|\t]+              {} /* skip blanks */
.                       {printf("[LEXER] ANOTHER UNKNOWN CHARACTER: %s\n", yytext);}

%%

