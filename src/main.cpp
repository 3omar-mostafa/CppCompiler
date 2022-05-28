#include <iostream>
#include <parser.hpp>
#include "nodes/Node.h"

extern FILE *yyin;
extern Node *programRoot;

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        yyin = fopen(argv[1], "r");
        if (yyin == nullptr)
        {
            fprintf(stderr, "error: could not open the input file %s", argv[1]);
            exit(-1);
        }
        std::cout << "Compiling " << argv[1] << std::endl;
    }
    else
    {
        std::cout << "No files were given, reading input from stdin" << std::endl;
    }

    yy::parser parser;
    parser.parse();

    std::cout << "Start Analyzing \n";
    bool analyze = programRoot->analyzeSemantic();
    std::cout << "Finished Analyzing with result = " << analyze << std::endl;
    if (!analyze)
    {
        std::cout << "Semantic Error Can not generate quadraples\n";
        return -1;
    }

    CodeGenerationHelper *genHelper = new CodeGenerationHelper();

    std::string quad = programRoot->generateCode(genHelper);
    std::cout << "Finished code generation with result = \n"
              << quad << std::endl;
    return 0;
}
