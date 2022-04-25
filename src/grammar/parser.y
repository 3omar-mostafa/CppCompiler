%require "3.2"
%language "c++"
%define api.value.type variant
%define api.token.constructor

%locations
%define api.location.file "location.hpp"

%define parse.trace
%define parse.error verbose
%define parse.lac full
%define parse.assert
%define api.token.prefix {TOKEN_}

%code requires {
#include <iostream>
#include <string>
#include <cmath>
}

%code {
// Forward declaration for yylex created by the lexer (flex)
yy::parser::symbol_type yylex();
}

%token <int> INTEGER
%token <double> REAL
%token <char> CHAR
%token <bool> BOOL
%token <std::string> STRING
%token <std::string> IDENTIFIER

%token TYPE_INT TYPE_FLOAT TYPE_CHAR TYPE_BOOL TYPE_VOID TYPE_STRING
%token CONST

%token IF ELSE SWITCH CASE DEFAULT FOR DO WHILE CONTINUE BREAK RETURN

%nterm <int> statement declaration_statement assignment_statement
%nterm <int> expr literal

%right '='
%left AND OR NOT
%left INC DEC
%left GE LE EQ NE '>' '<'
%left SHL SHR
%left '+' '-'
%left '*' '/'
%right POWER

%token EOF 0;
%%

%start program;

program:
    program statement   {}
|   /* */
;

statement:
    declaration_statement           {}
|   CONST declaration_statement     {}
|   assignment_statement            {}
;

declaration_statement:
    data_type IDENTIFIER ';' {$$=0; printf ("[PARSER] Defined an Identifier \n");}
|   data_type IDENTIFIER '=' expr ';' { $$ = $expr; printf ("[PARSER] Final Output: %d\n", $expr);}
;

assignment_statement:
    IDENTIFIER '=' expr ';' { $$ = $expr; printf ("[PARSER] Final Output: %d\n", $expr);}
;

data_type:
    TYPE_INT    {}
|   TYPE_FLOAT  {}
|   TYPE_CHAR   {}
|   TYPE_BOOL   {}
|   TYPE_VOID   {}
|   TYPE_STRING {}
;

expr[result]:
    literal                         {$result = $literal;}
|   expr[left] '+' expr[right]      { $result = $left + $right ;printf("[PARSER] Add result: %d\n", $result); std::cout << @result << std::endl;}
|   expr[left] '-' expr[right]      { $result = $left - $right ;printf("[PARSER] Sub result: %d\n", $result); std::cout << @result << std::endl;}
|   expr[left] '*' expr[right]      { $result = $left * $right ;printf("[PARSER] Mul result: %d\n", $result); std::cout << @result << std::endl;}
|   expr[left] '/' expr[right]      { $result = $left / $right ;printf("[PARSER] Div result: %d\n", $result); std::cout << @result << std::endl;}
|   expr[left] POWER expr[right]    { $result = pow($left,  $right); printf("[PARSER] power result: %d\n", $result); }
|   expr[left] AND expr[right]      { $result = $left && $right; printf("[PARSER] AND result: %d\n", $result); }
|   expr[left] OR expr[right]       { $result = $left || $right; printf("[PARSER] OR result: %d\n", $result); }
|   NOT expr[right]                 { $result = !$right;         printf("[PARSER] NOT result: %d\n", $result); }
|   expr[left] '>' expr[right]      { $result = $left > $right;  printf("[PARSER] > result: %d\n", $result); }
|   expr[left] GE expr[right]       { $result = $left >= $right; printf("[PARSER] >= result: %d\n", $result); }
|   expr[left] '<' expr[right]      { $result = $left < $right;  printf("[PARSER] < result: %d\n", $result); }
|   expr[left] LE expr[right]       { $result = $left <= $right; printf("[PARSER] <= result: %d\n", $result); }
|   expr[left] EQ expr[right]       { $result = $left == $right; printf("[PARSER] == result: %d\n", $result); }
|   expr[left] NE expr[right]       { $result = $left != $right; printf("[PARSER] != result: %d\n", $result); }
|   expr[left] SHL expr[right]      { $result = $left << $right; printf("[PARSER] << result: %d\n", $result); }
|   expr[left] SHR expr[right]      { $result = $left >> $right; printf("[PARSER] >> result: %d\n", $result); }
|   '(' expr[left] ')'              { $result = $left;           printf("[PARSER] >> result: %d\n", $result); }
;

literal:
    INTEGER                         { $literal = $INTEGER; printf("[PARSER] int result: %d\n", $literal);}
|   BOOL                            { $literal = $BOOL; printf("[PARSER] bool result: %d\n", $literal);}
|   REAL                            { $literal = $REAL; printf("[PARSER] real result: %d\n", $literal);}
|   CHAR                            { $literal = $CHAR; printf("[PARSER] char result: %d\n", $literal);}
%%


void yy::parser::error(const location_type &l, const std::string &err_message)
{
   std::cerr << "Error: " << err_message << " at " << l << "\n";
}
