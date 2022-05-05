    
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

yy::location loc; // Global location object, used to store the location cuurent token

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

"int"                   {printf("[LEXER] Data Type keyword: int\n"); return yy::parser::make_TYPE_INT(loc);}
"float"                 {printf("[LEXER] Data Type keyword: float\n"); return yy::parser::make_TYPE_FLOAT(loc);}
"char"                  {printf("[LEXER] Data Type keyword: char\n"); return yy::parser::make_TYPE_CHAR(loc);}
"bool"                  {printf("[LEXER] Data Type keyword: bool\n"); return yy::parser::make_TYPE_BOOL(loc);}
"void"                  {printf("[LEXER] Data Type keyword: void\n"); return yy::parser::make_TYPE_VOID(loc);}
"string"                {printf("[LEXER] Data Type keyword: string\n"); return yy::parser::make_TYPE_STRING(loc);}

"const"                 {printf("[LEXER] reserved keyword: const\n"); return yy::parser::make_CONST(loc);}

"if"                    {printf("[LEXER] reserved keyword: if\n"); return yy::parser::make_IF(loc);}
"else"                  {printf("[LEXER] reserved keyword: else\n"); return yy::parser::make_ELSE(loc);}
"switch"                {printf("[LEXER] reserved keyword: switch\n"); return yy::parser::make_SWITCH(loc);}
"case"                  {printf("[LEXER] reserved keyword: case\n"); return yy::parser::make_CASE(loc);}
"default"               {printf("[LEXER] reserved keyword: default\n"); return yy::parser::make_DEFAULT(loc);}
"for"                   {printf("[LEXER] reserved keyword: for\n"); return yy::parser::make_FOR(loc);}
"do"                    {printf("[LEXER] reserved keyword: do\n"); return yy::parser::make_DO(loc);}
"while"                 {printf("[LEXER] reserved keyword: while\n"); return yy::parser::make_WHILE(loc);}
"break"                 {printf("[LEXER] reserved keyword: break\n"); return yy::parser::make_BREAK(loc);}
"continue"              {printf("[LEXER] reserved keyword: continue\n"); return yy::parser::make_CONTINUE(loc);}
"return"                {printf("[LEXER] reserved keyword: return\n"); return yy::parser::make_RETURN(loc);}

"++"                    {printf("[LEXER] operator INC: ++\n"); return yy::parser::make_INC(loc);}
"--"                    {printf("[LEXER] operator DEC: --\n"); return yy::parser::make_DEC(loc);}
"=="                    {printf("[LEXER] operator EQUAL: ==\n"); return yy::parser::make_EQ(loc);}
"!="                    {printf("[LEXER] operator NOT_EQUAL: !=\n"); return yy::parser::make_NE(loc);}
">="                    {printf("[LEXER] operator GREATER_EQUAL: >=\n"); return yy::parser::make_GE(loc);}
"<="                    {printf("[LEXER] operator LESS_EQUAL: <=\n"); return yy::parser::make_LE(loc);}
"<<"                    {printf("[LEXER] operator SHIFT_LEFT: <<\n"); return yy::parser::make_SHL(loc);}
">>"                    {printf("[LEXER] operator SHIFT_RIGHT: >>\n"); return yy::parser::make_SHR(loc);}
"&&"|"and"              {printf("[LEXER] operator AND:: %s\n", yytext); return yy::parser::make_AND(loc);}
"||"|"or"               {printf("[LEXER] operator OR:: %s\n", yytext); return yy::parser::make_OR(loc);}
"!"|"not"               {printf("[LEXER] operator NOT: %s\n", yytext); return yy::parser::make_NOT(loc);}
"**"                    {printf("[LEXER] operator POWER: **\n"); return yy::parser::make_POWER(loc);}

[-+*/=;<>(){}[\],:]     {printf("[LEXER] ANOTHER operator: %s\n", yytext); return yy::parser::symbol_type(yytext[0], loc);}

"true"                  {printf("[LEXER] BOOLEAN TRUE: %s\n", yytext); return yy::parser::make_BOOL(true, loc);}
"false"                 {printf("[LEXER] BOOLEAN FALSE: %s\n", yytext); return yy::parser::make_BOOL(false, loc);}

{INTEGER}               {printf("[LEXER] INTEGER: %s\n", yytext); return yy::parser::make_INTEGER(atoi(yytext), loc);}
{REAL}                  {printf("[LEXER] REAL: %s\n", yytext);return yy::parser::make_REAL(atof(yytext), loc);}
{CHAR}                  {printf("[LEXER] CHAR: %s\n", yytext); return yy::parser::make_CHAR(yytext[1], loc);}
{STRING}                {printf("[LEXER] STRING: %s\n", yytext); return yy::parser::make_STRING(yytext, loc);}
{IDENTIFIER}            {printf("[LEXER] IDENTIFIER: %s\n", yytext);return yy::parser::make_IDENTIFIER(yytext, loc);}
{LINE_COMMENT}          {printf("[LEXER] SINGLE LINE COMMENT: %s\n", yytext);}
{MULTILINE_COMMENT}     {printf("[LEXER] MULTI_LINE COMMENT: %s\n", yytext);}
[ ]+                    {loc.step();} /* skip empty spaces */
[\t]+                   {   /*  
                                Skip tabs (assume tab = 4 spaces)
                                Note: we use 4-1 instead of 4 because eveery time yylex is called, it will increment the location
                            */
                            loc.columns(4-1); loc.step();
                        }
[\n]+                   {loc.step();} /* skip new lines */
[\r]                    {} /* ignore new lines of CRLF Line Endings to convert it to LF only */
<<EOF>>                 {printf("[LEXER] END OF FILE\n"); return yy::parser::make_EOF(loc);}
.                       {throw yy::parser::syntax_error(loc, "[LEXER] invalid character: " + std::string(yytext));}

%%

