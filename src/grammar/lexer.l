    
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
    %option yyclass="foo"
    It informs flex that you have derived `foo' as a subclass of yyFlexLexer so flex will place your actions in the member function `foo::yylex()' instead of `yyFlexLexer::yylex()'.
    It also generates a `yyFlexLexer::yylex()' member function that emits a run-time error (by invoking `yyFlexLexer::LexerError()') if called.

    %option yyclass="Lexer"
    */

    /*
    Makes the scanner not call `yywrap()' upon an end-of-file,
    but simply assume that there are no more files to scan
    (until the user points yyin at a new file and calls `yylex()' again)
    */
%option noyywrap

    /*
    Generate a C++ scanner class
    */
%option c++

%option warn

    /*
    Creates class FlexLexer, which provides an abstract base class defining the general scanner class interface.

    It provides the following member functions:
    `const char* YYText()'
        returns the text of the most recently matched token, the equivalent of yytext.
    `int YYLeng()'
        returns the length of the most recently matched token, the equivalent of yyleng.
    `int lineno() const'
        returns the current input line number (see `%option yylineno'), or 1 if `%option yylineno' was not used.
    `void set_debug( int flag )'
        sets the debugging flag for the scanner, equivalent to assigning to yy_flex_debug (see the Options section above).
        Note that you must build the scanner using `%option debug' to include debugging information in it.
    `int debug() const'
        returns the current setting of the debugging flag.
    */


    /*
    Creates another class yyFlexLexer, which is derived from FlexLexer.

    It defines the following additional member functions:
    `yyFlexLexer( istream* arg_yyin = 0, ostream* arg_yyout = 0 )'
        constructs a yyFlexLexer object using the given streams for input and output.
        If not specified, the streams default to cin and cout, respectively.
    `virtual int yylex()'
        performs the same role is `yylex()' does for ordinary flex scanners: it scans the input stream, consuming tokens, until a rule's action returns a value.
        If you derive a subclass S from yyFlexLexer and want to access the member functions and variables of S inside `yylex()', then you need to use `%option yyclass="S"' to inform flex that you will be using that subclass instead of yyFlexLexer. In this case, rather than generating `yyFlexLexer::yylex()', flex generates `S::yylex()' (and also generates a dummy `yyFlexLexer::yylex()' that calls `yyFlexLexer::LexerError()' if called).
    `virtual void switch_streams(istream* new_in = 0, ostream* new_out = 0)'
        reassigns yyin to new_in (if non-nil) and yyout to new_out (ditto), deleting the previous input buffer if yyin is reassigned.
    `int yylex( istream* new_in = 0, ostream* new_out = 0 )'
        first switches the input streams via `switch_streams( new_in, new_out )' and then returns the value of `yylex()'.

    */


%{
#include<iostream>
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

