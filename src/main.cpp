#include <iostream>
#include <parser.hpp>
extern FILE *yyin;

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        yyin = fopen(argv[1], "r");
        if (yyin == NULL)
        {
            fprintf(stderr, "error: could not open the input file %s", argv[1]);
            exit(-1);
        }
    }
    yy::parser parser;
    parser.parse();
    return 0;
}
