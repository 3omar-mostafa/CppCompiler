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
#include <algorithm>

}

%code {
// Forward declaration for yylex created by the lexer (flex)
yy::parser::symbol_type yylex();
Node* programRoot = nullptr;
}

// `provides` makes this section available to the lexer (code it put in the header file)
%code provides {
namespace yy{
    void printLocation(const yy::parser::location_type& location);
    std::string getLine(const std::string& filename, int line);
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

%token INC DEC

%token TYPE_INT TYPE_FLOAT TYPE_CHAR TYPE_BOOL TYPE_VOID TYPE_STRING
%token CONST

%token IF ELSE SWITCH CASE DEFAULT FOR DO WHILE CONTINUE BREAK RETURN

%nterm <Node*> statement
%nterm <StmtListNode*> statement_list
%nterm <VariableDeclarationNode*> variable_declaration_statement variable_declaration
%nterm <Node*> loop_statement
%nterm <SwitchCondNode*> switch_statement
%nterm <CaseNode*> case_statement
%nterm <IfElseCondNode*> if_statement
%nterm <ExpressionNode*> expr
%nterm <LiteralNode*> literal
%nterm <FunctionDeclarationNode*> function_declaration_statemnt
%nterm <ReturnNode*> return_statement
%nterm <ExpressionList*> arguments func_args
%nterm <VarDecList*> parameters func_params
%nterm <DataType> data_type
%nterm <Node*> program

%right '='
%left AND OR NOT
%left GE LE EQ NE '>' '<'
%left SHL SHR
%left '+' '-'
%left '*' '/'
%right POWER
%right UNARY_MINUS
%right PRE_INC PRE_DEC
%left POST_INC POST_DEC

%token EOF 0;

%nonassoc IFUNMATCHED 
%nonassoc ELSE 
%%

%start program;

program:
    statement_list      { $program = nullptr; programRoot = new StmtBlockNode(@$, *$statement_list); delete $statement_list; $statement_list = nullptr; }
|   /* empty */         { $program = nullptr; programRoot = new StmtBlockNode(@$);}
;

statement:
    variable_declaration_statement  {$statement = $variable_declaration_statement;}
|   if_statement                    {$statement = $if_statement;}
|   switch_statement                {$statement = $switch_statement;}
|   case_statement                  {$statement = $case_statement;}
|   loop_statement                  {$statement = $loop_statement;}
|   function_declaration_statemnt   {$statement = $function_declaration_statemnt;}
|   return_statement                {$statement = $return_statement;}
|   '{''}'                          {$statement = new StmtBlockNode(@$);}
|   '{'statement_list'}'            {$statement = new StmtBlockNode(@$, *$statement_list); delete $statement_list; $statement_list = nullptr;}
|   expr ';'                        {$statement = $expr;}
;

statement_list:
    statement                           {$$ = new StmtListNode(); $$->push_back($statement);}
|   statement_list[rhs] statement       {$$ = $rhs; $$->push_back($statement);}
;

variable_declaration_statement:
    variable_declaration ';'            { $$ = $variable_declaration;}
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
    SWITCH '(' expr ')' statement           {$$ = new SwitchCondNode(@$, $expr, $statement);}
;

case_statement:
    CASE expr ':' statement                 {$$ = new CaseNode(@$, $expr, $statement);}
|   DEFAULT ':' statement                   {$$ = new CaseNode(@$, NULL, $statement);}
;

loop_statement:
    FOR '(' variable_declaration_statement[varDec] expr[cond] ';' expr[step] ')' statement  	{$$ = new ForLoopNode(@$, $varDec, $cond, $step, $statement);}
|   WHILE '(' expr ')' statement                                        			{$$ = new WhileLoopNode(@$, $expr, $statement);}
|   DO statement WHILE '(' expr ')' ';'                                 			{$$ = new DoWhileLoopNode(@$, $expr, $statement);}
|   CONTINUE ';'                                                        			{$$ = new ContinueNode(@$);}
|   BREAK ';'                                                           			{$$ = new BreakNode(@$);}
;

function_declaration_statemnt:
    data_type IDENTIFIER '(' func_params ')' statement	{   $$ = new FunctionDeclarationNode(@$, $data_type, $IDENTIFIER, *$func_params, $statement);
                                                            delete $func_params;
                                                            $func_params = nullptr; }
;

parameters:
   variable_declaration                         	{$$ = new VarDecList(); $$->push_back($variable_declaration);}
|    parameters[rhs] ',' variable_declaration       {$$ = $rhs; $$->push_back($variable_declaration);}
;

func_params:
    parameters                	{$$ = $parameters;}
|   /* empty */               	{$$ = new VarDecList();}
;

arguments:
   expr                        	{$$ = new ExpressionList(); $$->push_back($expr);}
|    arguments[rhs] ',' expr   	{$$ = $rhs; $$->push_back($expr);}
;

func_args:
    arguments             	    {$$ = $arguments;}
|   /* empty */                 {$$ = new ExpressionList();}
;

return_statement:
    RETURN ';'              {$$ = new ReturnNode(@$, NULL);}
|   RETURN expr ';'         {$$ = new ReturnNode(@$, $expr);}
;

data_type:
    TYPE_INT    { $data_type = DTYPE_INT;}
|   TYPE_FLOAT  { $data_type = DTYPE_FLOAT;}
|   TYPE_CHAR   { $data_type = DTYPE_CHAR;}
|   TYPE_BOOL   { $data_type = DTYPE_BOOL;}
|   TYPE_VOID   { $data_type = DTYPE_VOID; }
;

expr[result]:
    literal                         	{ $result = $literal;}
|   expr[left] '+' expr[right]      	{ $result = new BinaryOpNode(@$, $left, OPR_ADD, $right);}
|   expr[left] '-' expr[right]      	{ $result = new BinaryOpNode(@$, $left, OPR_SUB, $right);}
|   '-' expr[right] %prec UNARY_MINUS 	{ $result = new UnaryOpNode(@$, OPR_UNARY_MINUS, $right);}
|   expr[left] '*' expr[right]      	{ $result = new BinaryOpNode(@$, $left, OPR_MUL, $right);}
|   expr[left] '/' expr[right]      	{ $result = new BinaryOpNode(@$, $left, OPR_DIV, $right);}
|   expr[left] POWER expr[right]    	{ $result = new BinaryOpNode(@$, $left, OPR_POW, $right);}
|   expr[left] AND expr[right]      	{ $result = new BinaryOpNode(@$, $left, OPR_AND, $right);}
|   expr[left] OR expr[right]       	{ $result = new BinaryOpNode(@$, $left, OPR_OR, $right);}
|   NOT expr[right]                 	{ $result = new UnaryOpNode(@$, OPR_NOT, $right);}
|   expr[left] '>' expr[right]      	{ $result = new BinaryOpNode(@$, $left, OPR_GREATER, $right);}
|   expr[left] GE expr[right]       	{ $result = new BinaryOpNode(@$, $left, OPR_GREATER_EQUAL, $right);}
|   expr[left] '<' expr[right]      	{ $result = new BinaryOpNode(@$, $left, OPR_LESS, $right);}
|   expr[left] LE expr[right]       	{ $result = new BinaryOpNode(@$, $left, OPR_LESS_EQUAL, $right);}
|   expr[left] EQ expr[right]       	{ $result = new BinaryOpNode(@$, $left, OPR_EQUAL, $right);}
|   expr[left] NE expr[right]       	{ $result = new BinaryOpNode(@$, $left, OPR_NOT_EQUAL, $right);}
|   expr[left] SHL expr[right]      	{ $result = new BinaryOpNode(@$, $left, OPR_SHL, $right);}
|   expr[left] SHR expr[right]      	{ $result = new BinaryOpNode(@$, $left, OPR_SHR, $right);}
|   '(' expr[left] ')'              	{ $result = $left;}
|   IDENTIFIER                      	{ $result = $IDENTIFIER;}
|   IDENTIFIER '=' expr[right]      	{ $result = new AssignOpNode(@$, $IDENTIFIER, $right);}
|   IDENTIFIER '(' func_args ')'    	{ $result = new FunctionCallNode(@$, $IDENTIFIER, *$func_args); delete $func_args; $func_args = nullptr;}
|   INC IDENTIFIER %prec PRE_INC        { $result = new UnaryOpNode(@$, OPR_PRE_INC, $IDENTIFIER);}
|   DEC IDENTIFIER %prec PRE_DEC        { $result = new UnaryOpNode(@$, OPR_PRE_DEC, $IDENTIFIER);}
|   IDENTIFIER INC %prec POST_INC       { $result = new UnaryOpNode(@$, OPR_POST_INC, $IDENTIFIER);}
|   IDENTIFIER DEC %prec POST_DEC       { $result = new UnaryOpNode(@$, OPR_POST_DEC, $IDENTIFIER);}
;

literal:
    INTEGER                         { $literal = new LiteralNode(@$, $INTEGER, DTYPE_INT);}
|   BOOL                            { $literal = new LiteralNode(@$, std::to_string($BOOL), DTYPE_BOOL);}
|   REAL                            { $literal = new LiteralNode(@$, $REAL, DTYPE_FLOAT);}
|   CHAR                            { $literal = new LiteralNode(@$, std::to_string($CHAR), DTYPE_CHAR);}
%%


void yy::parser::error(const location_type &loc, const std::string &err_message)
{
   Utils::log(err_message, loc, "error");
   exit(1);
}
