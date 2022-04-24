    
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


%{
#include <iostream>
#include <string>
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

"int"                   {printf("Found Data Type keyword: int\n");}
"float"                 {printf("Found Data Type keyword: float\n");}
"char"                  {printf("Found Data Type keyword: char\n");}
"bool"                  {printf("Found Data Type keyword: bool\n");}
"void"                  {printf("Found Data Type keyword: void\n");}
"string"                {printf("Found Data Type keyword: string\n");}

"const"                 {printf("Found reserved keyword: const\n");}
"if"                    {printf("Found reserved keyword: if\n");}
"else"                  {printf("Found reserved keyword: else\n");}
"switch"                {printf("Found reserved keyword: switch\n");}
"case"                  {printf("Found reserved keyword: case\n");}
"default"               {printf("Found reserved keyword: default\n");}
"for"                   {printf("Found reserved keyword: for\n");}
"do"                    {printf("Found reserved keyword: do\n");}
"while"                 {printf("Found reserved keyword: while\n");}
"break"                 {printf("Found reserved keyword: break\n");}
"continue"              {printf("Found reserved keyword: continue\n");}
"return"                {printf("Found reserved keyword: return\n");}


"++"                    {printf("Found operator INC: ++\n");}
"--"                    {printf("Found operator DEC: --\n");}
"=="                    {printf("Found operator EQUAL: ==\n");}
"!="                    {printf("Found operator NOT_EQUAL: !=\n");}
">="                    {printf("Found operator GREATER_EQUAL: >=\n");}
"<="                    {printf("Found operator LESS_EQUAL: <=\n");}
"<<"                    {printf("Found operator SHIFT_LEFT: <<\n");}
">>"                    {printf("Found operator SHIFT_RIGHT: >>\n");}
"&&"|"and"              {printf("Found operator AND:: %s\n", yytext);}
"||"|"or"               {printf("Found operator OR:: %s\n", yytext);}
"!"|"not"               {printf("Found operator NOT: %s\n", yytext);}
"**"                    {printf("Found operator POWER: **\n");}
"+"                     {printf("Found operator PLUS: +\n");}
"-"                     {printf("Found operator MINUS: -\n");}
"*"                     {printf("Found operator MULTIPLY: *\n");}
"/"                     {printf("Found operator DIVISON: /\n");}

[=<>(){}[\],:;]         {printf("Found ANOTHER operator: %s\n", yytext);}


{INTEGER}               {printf("Found INTEGER: %s\n", yytext);}
{REAL}                  {printf("Found REAL: %s\n", yytext);}
{CHAR}                  {printf("Found CHAR: %s\n", yytext);}
{STRING}                {printf("Found STRING: %s\n", yytext);}
"true"                  {printf("Found BOOLEAN TRUE: %s\n", yytext);}
"false"                 {printf("Found BOOLEAN FALSE: %s\n", yytext);}
{IDENTIFIER}            {printf("Found IDENTIFIER: %s\n", yytext);}
{LINE_COMMENT}          {printf("Found SINGLE LINE COMMENT: %s\n", yytext);}
{MULTILINE_COMMENT}     {printf("Found MULTI_LINE COMMENT: %s\n", yytext);}
[ |\n|\t]+              {} /* skip blanks */
.                       {printf("Found ANOTHER UNKNOWN CHARACTER: %s\n", yytext);}

%%

