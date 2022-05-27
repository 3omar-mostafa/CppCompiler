// Bison minimum version
%require "3.0"

// Generate C++ files instead of C
%language "c++"

// Enable C++ Variants (Allow using C++ data types in token data types)
// i.e. use `%token <std::string> IDENTIFIER` instead of union
// Documentation: https://www.gnu.org/software/bison/manual/html_node/C_002b_002b-Variants.html
%define api.value.type variant

// Request that symbols be handled as a whole (type, value, location) in the lexer
// i.e. use `return yy::parser::make_IDENTIFIER(yytext, loc)` in lexer
%define api.token.constructor

// Enable Location Tracking
// Documentation: https://www.gnu.org/software/bison/manual/html_node/Tracking-Locations.html
// Documentation: https://www.gnu.org/software/bison/manual/html_node/C_002b_002b-Location-Values.html
%locations

// The name of location file to generate
%define api.location.file "location.hpp"

// Enable parser tracing and detailed error messages
%define parse.trace
%define parse.error verbose
%define parse.lac full

// This option checks that symbols must be constructed and destroyed properly.
%define parse.assert

// Add a prefix to the token names to avoid conflicts.
%define api.token.prefix {TOKEN_}

%code requires {
#include "headers.h"

}

%code {
// Forward declaration for yylex created by the lexer (flex)
yy::parser::symbol_type yylex();
Node* programRoot = NULL;
}

// `provides` makes this section available to the lexer (code it put in the header file)
%code provides {
namespace yy{
    void printLocation(const yy::parser::location_type& location);
}
}

%code {
namespace yy{
    void printLocation(const yy::parser::location_type& location) {
        printf(" [@ %d:%d -> %d:%d]\n", location.begin.line, location.begin.column, location.end.line, location.end.column);
    }
}
}

%token <std::string> INTEGER
%token <std::string> REAL
%token <char> CHAR
%token <bool> BOOL
%token <std::string> STRING
%token <IdentifierNode*> IDENTIFIER

%token TYPE_INT TYPE_FLOAT TYPE_CHAR TYPE_BOOL TYPE_VOID TYPE_STRING
%token CONST

%token IF ELSE SWITCH CASE DEFAULT FOR DO WHILE CONTINUE BREAK RETURN

%nterm <Node*> statement statement_list
%nterm <VariableDeclarationNode*> variable_declaration_statement variable_declaration
%nterm <Node*> switch_statement case_statemnts loop_statement
%nterm <IfElseCondNode*> if_statement
%nterm <ExpressionNode*> expr
%nterm <LiteralNode*> literal
%nterm <int> function_declaration_statemnt return_statement
%nterm <int> arguments func_args
%nterm <int> parameters func_params
%nterm <DataType> data_type
%nterm <Node*> program

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
    statement_list      { $program = $statement_list; programRoot = $statement_list;}
|   /* empty */         {}
;

statement:
    variable_declaration_statement  {$statement = $variable_declaration_statement;}
|   if_statement                    {$statement = $if_statement;}
|   switch_statement                {}
|   loop_statement                  {$statement = $loop_statement;}
|   function_declaration_statemnt   {}
|   return_statement                {}
|   '{''}'                          {}
|   '{'statement_list'}'            {$statement = $statement_list;}
|   expr ';'                        {$statement = $expr;}
;

statement_list:
    statement                       {$statement_list = $statement;}
|   statement_list statement        {}
;

variable_declaration_statement:
    variable_declaration ';'        { $$ = $variable_declaration;}
;

variable_declaration:
    data_type IDENTIFIER                { $$ = new VariableDeclarationNode(@$, $data_type, $IDENTIFIER); }
|   data_type IDENTIFIER '=' expr       { $$ = new VariableDeclarationNode(@$, $data_type, $IDENTIFIER, $expr); }
|   CONST data_type IDENTIFIER '=' expr { $$ = new VariableDeclarationNode(@$, $data_type, $IDENTIFIER, $expr, true); }
;

if_statement:
    IF '(' expr ')' statement %prec IFUNMATCHED                     {$$ = new IfElseCondNode(@$,$expr , $statement);}
|   IF '(' expr ')' statement[if_body] ELSE statement[else_body]    {$$ = new IfElseCondNode(@$,$expr , $if_body, $else_body);}
;

switch_statement:
    SWITCH '(' expr ')' '{' case_statemnts '}'      {}
;

case_statemnts:
    case_statemnts CASE expr ':' statement  {}
|   CASE expr ':' statement                 {}
;

loop_statement:
    FOR '(' variable_declaration_statement expr ';' expr ')' statement  {}
|   WHILE '(' expr ')' statement                                        {$$ = new WhileLoopNode(@$, $expr, $statement);}
|   DO statement WHILE '(' expr ')' ';'                                 {}
|   CONTINUE ';'                                                        {}
|   BREAK ';'                                                           {}
;

function_declaration_statemnt:
    data_type IDENTIFIER '(' func_params ')' statement          {}
;

parameters:
    parameters ',' variable_declaration      {}
|   variable_declaration                     {}
;

func_params:
    parameters               {}
|   /* empty */                    {}
;

arguments:
    arguments ',' expr      {}
|   expr                    {}
;

func_args:
    arguments               {}
|   /* empty */                   {}
;

return_statement:
    RETURN ';'              {}
|   RETURN expr ';'         {}

data_type:
    TYPE_INT    { $data_type = DTYPE_INT;}
|   TYPE_FLOAT  { $data_type = DTYPE_FLOAT;}
|   TYPE_CHAR   { $data_type = DTYPE_CHAR;}
|   TYPE_BOOL   { $data_type = DTYPE_BOOL;}
|   TYPE_VOID   { $data_type = DTYPE_VOID; }
;

expr[result]:
    literal                         { $result = $literal;}
|   expr[left] '+' expr[right]      { $result = new BinaryOpNode(@$, $left, OPR_ADD, $right);}
|   expr[left] '-' expr[right]      { $result = new BinaryOpNode(@$, $left, OPR_SUB, $right);}
|   expr[left] '*' expr[right]      { $result = new BinaryOpNode(@$, $left, OPR_MUL, $right);}
|   expr[left] '/' expr[right]      { $result = new BinaryOpNode(@$, $left, OPR_DIV, $right);}
|   expr[left] POWER expr[right]    { $result = new BinaryOpNode(@$, $left, OPR_POW, $right);}
|   expr[left] AND expr[right]      { $result = new BinaryOpNode(@$, $left, OPR_AND, $right);}
|   expr[left] OR expr[right]       { $result = new BinaryOpNode(@$, $left, OPR_OR, $right);}
|   NOT expr[right]                 { $result = new UnaryOpNode(@$, OPR_NOT, $right);}
|   expr[left] '>' expr[right]      { $result = new BinaryOpNode(@$, $left, OPR_GREATER, $right);}
|   expr[left] GE expr[right]       { $result = new BinaryOpNode(@$, $left, OPR_GREATER_EQUAL, $right);}
|   expr[left] '<' expr[right]      { $result = new BinaryOpNode(@$, $left, OPR_LESS, $right);}
|   expr[left] LE expr[right]       { $result = new BinaryOpNode(@$, $left, OPR_LESS_EQUAL, $right);}
|   expr[left] EQ expr[right]       { $result = new BinaryOpNode(@$, $left, OPR_EQUAL, $right);}
|   expr[left] NE expr[right]       { $result = new BinaryOpNode(@$, $left, OPR_NOT_EQUAL, $right);}
|   expr[left] SHL expr[right]      { $result = new BinaryOpNode(@$, $left, OPR_SHL, $right);}
|   expr[left] SHR expr[right]      { $result = new BinaryOpNode(@$, $left, OPR_SHR, $right);}
|   '(' expr[left] ')'              { $result = $left;}
|   IDENTIFIER                      { $result = $IDENTIFIER;}
|   IDENTIFIER '=' expr[right]       { $result = new AssignOpNode(@$, $IDENTIFIER, $right);}
|   IDENTIFIER '(' func_args ')'    { $result = 0;                  }
;

literal:
    INTEGER                         { $literal = new LiteralNode(@$, $INTEGER, DTYPE_INT);}
|   BOOL                            { $literal = new LiteralNode(@$, std::to_string($BOOL), DTYPE_BOOL);}
|   REAL                            { $literal = new LiteralNode(@$, $REAL, DTYPE_FLOAT);}
|   CHAR                            { $literal = new LiteralNode(@$, std::to_string($CHAR), DTYPE_CHAR);}
%%


void yy::parser::error(const location_type &l, const std::string &err_message)
{
   std::cerr << "Error: " << err_message << " at " << l << "\n";
}
