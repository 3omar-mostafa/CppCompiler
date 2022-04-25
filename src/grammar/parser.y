%require "3.2"
%language "c++"
%define api.value.type variant
%define api.token.constructor
// %locations
%define parse.trace
%define parse.error verbose
%define parse.lac full
%define parse.assert

%code requires {
#include <iostream>
#include <string>
}

%code {
// Forward declaration for yylex created by the lexer (flex)
yy::parser::symbol_type yylex();
}

%token <int> INTEGER
%token <std::string> STRING
%token <std::string> IDENTIFIER
%nterm <int> statement
%nterm <int> expr
%token PLUS "+"
%token MINUS "-"
%token MULTIPLY "*"
%token DIVISION "/"
%token ASSIGN "="
%token SEMICOLON ";"

%right "="
%left "+" "-";
%left "*" "/";


%%

%start statement;

statement:
    IDENTIFIER "=" expr ";" { $statement = $expr; printf ("[PARSER] Final Output: %d\n", $expr);}
;

expr[result]:
    INTEGER                         { $result = $INTEGER;}
|   expr[left] "+" expr[right]      { $result = $left + $right ;printf("[PARSER] Add result: %d\n", $result); }
|   expr[left] "-" expr[right]      { $result = $left - $right ;printf("[PARSER] Sub result: %d\n", $result); }
|   expr[left] "*" expr[right]      { $result = $left * $right ;printf("[PARSER] Mul result: %d\n", $result); }
|   expr[left] "/" expr[right]      { $result = $left / $right ;printf("[PARSER] Div result: %d\n", $result); }
;

%%


void yy::parser::error(const std::string &err_message) {
   std::cerr << "Error: " << err_message << "\n";
}

// void yy::parser::error(const location_type &l, const std::string &err_message)
// {
//    std::cerr << "Error: " << err_message << " at " << l << "\n";
// }
