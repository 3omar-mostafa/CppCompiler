#include <iostream>
#include <FlexLexer.h>

int main() {
    FlexLexer* lexer = new yyFlexLexer;
    lexer->set_debug(1);
    while (lexer->yylex() != 0);
    return 0;
}

