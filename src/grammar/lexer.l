
    /*
    Comments can start with // or /*
    But MUST be indented, or have at least one space before the comment.
    */

    /*
    Options Documentation: https://ftp.gnu.org/old-gnu/Manuals/flex-2.5.4/html_node/flex_17.html
    */

    /*
    Makes the generated scanner run in debug mode. Whenever a pattern is recognized and the global yy_flex_debug is non-zero (which is the default), the scanner will write to stderr a line of the form:
    --accepting rule at line 53 ("the matched text")
    The line number refers to the location of the rule in the file defining the scanner (i.e., the file that was fed to flex).
    Messages are also generated when the scanner backs up, accepts the default rule, reaches the end of its input buffer (or encounters a NUL; at this point, the two look the same as far as the scanner's concerned), or reaches an end-of-file.
    */

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
    /*
    Disable interactive mode (read line by line from standard input)
    This removes calls `isatty()` function, which makes it compatible with Microsoft Visual Studio
    */
%option never-interactive

    /*
    Disable including <unistd.h> header file, which is only available at linux
    Which makes it compatible with Microsoft Visual Studio
    Note: This is equivalent to #define YY_NO_UNISTD_H
    */
%option nounistd

%{
#include <iostream>
#include <string>

#include "parser.hpp"
#include "location.hpp"

#undef YY_DECL
// Give Flex the prototype of yylex we want which is compatible with Bison C++ variants
#define YY_DECL yy::parser::symbol_type yylex()


int countLines(std::string text) {
    int lines = 0;
    for(char c: text) {
        lines += c == '\n';
    }
    return lines;
}

std::string yyfilename;

yy::location loc; // Global location object, used to store the location current token

#define YY_USER_INIT loc.initialize(&yyfilename)

// Code run each time a pattern is matched.
// Move the location forward by the length of the matched token
// and move the current line number forward by the number of newlines
#define YY_USER_ACTION loc.columns(yyleng); loc.lines(countLines(yytext));

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

%{
// Code run each time yylex is called.
// This section MUST be inside %% not before it to include it inside yylex function

// Move the location one step forward
// Note that the step is not 1, it is the length of the last matched token
loc.step();
%}

"int"                   {return yy::parser::make_TYPE_INT(loc);}
"float"                 {return yy::parser::make_TYPE_FLOAT(loc);}
"char"                  {return yy::parser::make_TYPE_CHAR(loc);}
"bool"                  {return yy::parser::make_TYPE_BOOL(loc);}
"void"                  {return yy::parser::make_TYPE_VOID(loc);}
"string"                {return yy::parser::make_TYPE_STRING(loc);}

"const"                 {return yy::parser::make_CONST(loc);}

"if"                    {return yy::parser::make_IF(loc);}
"else"                  {return yy::parser::make_ELSE(loc);}
"switch"                {return yy::parser::make_SWITCH(loc);}
"case"                  {return yy::parser::make_CASE(loc);}
"default"               {return yy::parser::make_DEFAULT(loc);}
"for"                   {return yy::parser::make_FOR(loc);}
"do"                    {return yy::parser::make_DO(loc);}
"while"                 {return yy::parser::make_WHILE(loc);}
"break"                 {return yy::parser::make_BREAK(loc);}
"continue"              {return yy::parser::make_CONTINUE(loc);}
"return"                {return yy::parser::make_RETURN(loc);}

"++"                    {return yy::parser::make_INC(loc);}
"--"                    {return yy::parser::make_DEC(loc);}
"=="                    {return yy::parser::make_EQ(loc);}
"!="                    {return yy::parser::make_NE(loc);}
">="                    {return yy::parser::make_GE(loc);}
"<="                    {return yy::parser::make_LE(loc);}
"<<"                    {return yy::parser::make_SHL(loc);}
">>"                    {return yy::parser::make_SHR(loc);}
"&&"|"and"              {return yy::parser::make_AND(loc);}
"||"|"or"               {return yy::parser::make_OR(loc);}
"!"|"not"               {return yy::parser::make_NOT(loc);}
"**"                    {return yy::parser::make_POWER(loc);}

[-+*/=;<>(){}[\],:]     {return yy::parser::symbol_type(yytext[0], loc);}

"true"                  {return yy::parser::make_BOOL(true, loc);}
"false"                 {return yy::parser::make_BOOL(false, loc);}

{INTEGER}               {return yy::parser::make_INTEGER(yytext, loc);}
{REAL}                  {return yy::parser::make_REAL(yytext, loc);}
{CHAR}                  {return yy::parser::make_CHAR(yytext[1], loc);}
{STRING}                {return yy::parser::make_STRING(yytext, loc);}
{IDENTIFIER}            {return yy::parser::make_IDENTIFIER(new IdentifierNode(loc, yytext), loc);}
{LINE_COMMENT}          {}
{MULTILINE_COMMENT}     {}
[ ]+                    {loc.step();} /* skip empty spaces */
[\t]+                   {   /*  
                                Skip tabs (assume tab = 4 spaces)
                                Note: tab is treated as one character, but we want to treat it as 4
                                So we increment the location by 3 times number of tab characters,
                                not by 4 times because every time yylex is called, it will increment the location
                            */
                            loc.columns(3 * yyleng); loc.step();
                        }
[\n]+                   {loc.step();} /* skip new lines */
[\r]                    {} /* ignore new lines of CRLF Line Endings to convert it to LF only */
<<EOF>>                 {return yy::parser::make_EOF(loc);}
.                       {throw yy::parser::syntax_error(loc, "[LEXER] invalid character: " + std::string(yytext));}

%%

