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

%code {
namespace yy{
    void printLocation(const yy::parser::location_type& location) {
        printf(" [@ %d:%d -> %d:%d]\n", location.begin.line, location.begin.column, location.end.line, location.end.column);
    }
}
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

%nterm <int> statement statement_list variable_declaration_statement variable_declaration 
%nterm <int> if_statement switch_statement case_statemnts loop_statement
%nterm <int> expr literal
%nterm <int> function_declaration_statemnt return_statement
%nterm <int> arguments func_args
%nterm <int> parameters func_params

%right '='
%left AND OR NOT
%left INC DEC
%left GE LE EQ NE '>' '<'
%left SHL SHR
%left '+' '-'
%left '*' '/'
%right POWER

%token EOF 0;

%nonassoc IFUNMATCHED 
%nonassoc ELSE 
%%

%start program;

program:
    program statement   {}
|   /* */
;

statement:
    variable_declaration_statement  {}
|   if_statement                    {}
|   switch_statement                {}
|   loop_statement                  {}
|   function_declaration_statemnt   {}
|   return_statement                {}
|   '{''}'                          {}
|   '{'statement_list'}'            {}
|   expr ';'                        {}
;

statement_list:
    statement                       {}
|   statement_list statement        {}
;

variable_declaration_statement:
    variable_declaration ';'        {}
;

variable_declaration:
    data_type IDENTIFIER                { $$=0; printf ("[PARSER] Defined an Identifier"); printLocation(@$);}
|   data_type IDENTIFIER '=' expr       { $$ = $expr; printf ("[PARSER] Declartion Final Output: %d", $expr); printLocation(@$);}
|   CONST data_type IDENTIFIER '=' expr { $$ = $expr; printf ("[PARSER] Const Declartion Final Output: %d", $expr); printLocation(@$);}
;

if_statement:
    IF '(' expr ')' statement %prec IFUNMATCHED    {printf("[PARSER] if statement"); printLocation(@$);}
|   IF '(' expr ')' statement ELSE statement       {printf("[PARSER] if-else"); printLocation(@$);}
;

switch_statement:
    SWITCH '(' expr ')' '{' case_statemnts '}'      {printf("[PARSER] switch-case"); printLocation(@$);}
;

case_statemnts:
    case_statemnts CASE expr ':' statement  {}
|   CASE expr ':' statement                 {}
;

loop_statement:
    FOR '(' variable_declaration_statement expr ';' expr ')' statement  {printf("[PARSER] for-loop"); printLocation(@$);}
|   WHILE '(' expr ')' statement                                        {printf("[PARSER] while-loop"); printLocation(@$);}
|   DO statement WHILE '(' expr ')' ';'                                 {printf("[PARSER] do-while"); printLocation(@$);}
|   CONTINUE ';'                                                        {printf("[PARSER] continue"); printLocation(@$);}
|   BREAK ';'                                                           {printf("[PARSER] break"); printLocation(@$);}
;

function_declaration_statemnt:
    data_type IDENTIFIER '(' func_params ')' statement          {printf("[PARSER] Function declaration"); printLocation(@$);}
;

parameters:
    parameters ',' variable_declaration      {}
|   variable_declaration                     {}
;

func_params:
    parameters               {}
|   /* */                    {}
;

arguments:
    arguments ',' expr      {}
|   expr                    {}
;

func_args:
    arguments               {}
|   /* */                   {}
;

return_statement:
    RETURN ';'              {}
|   RETURN expr ';'         {}

data_type:
    TYPE_INT    {}
|   TYPE_FLOAT  {}
|   TYPE_CHAR   {}
|   TYPE_BOOL   {}
|   TYPE_VOID   {}
|   TYPE_STRING {}
;

expr[result]:
    literal                         { $result = $literal;}
|   expr[left] '+' expr[right]      { $result = $left + $right;         printf("[PARSER] Add result: %d", $result); printLocation(@$);}
|   expr[left] '-' expr[right]      { $result = $left - $right;         printf("[PARSER] Sub result: %d", $result); printLocation(@$);}
|   expr[left] '*' expr[right]      { $result = $left * $right;         printf("[PARSER] Mul result: %d", $result); printLocation(@$);}
|   expr[left] '/' expr[right]      { $result = $left / $right;         printf("[PARSER] Div result: %d", $result); printLocation(@$);}
|   expr[left] POWER expr[right]    { $result = pow($left,  $right);    printf("[PARSER] power result: %d", $result); printLocation(@$);}
|   expr[left] AND expr[right]      { $result = $left && $right;        printf("[PARSER] AND result: %d", $result); printLocation(@$);}
|   expr[left] OR expr[right]       { $result = $left || $right;        printf("[PARSER] OR result: %d", $result); printLocation(@$);}
|   NOT expr[right]                 { $result = !$right;                printf("[PARSER] NOT result: %d", $result); printLocation(@$);}
|   expr[left] '>' expr[right]      { $result = $left > $right;         printf("[PARSER] > result: %d", $result); printLocation(@$);}
|   expr[left] GE expr[right]       { $result = $left >= $right;        printf("[PARSER] >= result: %d", $result); printLocation(@$);}
|   expr[left] '<' expr[right]      { $result = $left < $right;         printf("[PARSER] < result: %d", $result); printLocation(@$);}
|   expr[left] LE expr[right]       { $result = $left <= $right;        printf("[PARSER] <= result: %d", $result); printLocation(@$);}
|   expr[left] EQ expr[right]       { $result = $left == $right;        printf("[PARSER] == result: %d", $result); printLocation(@$);}
|   expr[left] NE expr[right]       { $result = $left != $right;        printf("[PARSER] != result: %d", $result); printLocation(@$);}
|   expr[left] SHL expr[right]      { $result = $left << $right;        printf("[PARSER] << result: %d", $result); printLocation(@$);}
|   expr[left] SHR expr[right]      { $result = $left >> $right;        printf("[PARSER] >> result: %d", $result); printLocation(@$);}
|   '(' expr[left] ')'              { $result = $left;                  printf("[PARSER] result: %d", $result); printLocation(@$);}
|   IDENTIFIER                      { $result = 0;                      printf("[PARSER] Identifier"); printLocation(@$);}
|   IDENTIFIER '=' expr[left]       { $result = 0;                      printf("[PARSER] Identifier=expr"); printLocation(@$);}
|   IDENTIFIER '(' func_args ')'    { $result = 0;                      printf("[PARSER] Function call"); printLocation(@$);}
;

literal:
    INTEGER                         { $literal = $INTEGER; printf("[PARSER] int result: %d", $literal); printLocation(@$);}
|   BOOL                            { $literal = $BOOL; printf("[PARSER] bool result: %d", $literal); printLocation(@$);}
|   REAL                            { $literal = $REAL; printf("[PARSER] real result: %d", $literal); printLocation(@$);}
|   CHAR                            { $literal = $CHAR; printf("[PARSER] char result: %c", $literal); printLocation(@$);}
%%


void yy::parser::error(const location_type &l, const std::string &err_message)
{
   std::cerr << "Error: " << err_message << " at " << l << "\n";
}
