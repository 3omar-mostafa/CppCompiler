#include <iostream>
#include <fstream>
#include <parser.hpp>
#include "nodes/Node.h"

extern FILE *yyin;
extern Node *programRoot;

void writeToFile(string data, string filename);

int main(int argc, char *argv[])
{
    if (argc >= 2)
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

    AnalysisHelper *analysisHelper = new AnalysisHelper(argv[1]);

    std::cout << "Start Analyzing \n";
    bool analyze = programRoot->analyzeSemantic(analysisHelper);
    if (!analyze)
    {
        std::cout << "Semantic Error Can not generate quadraples\n";
        return 0;
    }
    std::cout << "Finished Semantic analysis\n";
    CodeGenerationHelper *genHelper = new CodeGenerationHelper();

    std::string quad = programRoot->generateCode(genHelper);

    std::string outFile;
    if (argc == 3)
    {
        outFile = argv[2];
    }
    else
    {
        outFile = "out.txt";
    }

    writeToFile(quad, outFile);

    std::cout << "Finished code generation with result = \n";
    //   << quad << std::endl;

    return 0;
}

void writeToFile(string data, string filename)
{
    if (filename.empty())
    {
        return;
    }

    ofstream fout(filename);

    if (!fout.is_open())
    {
        fprintf(stderr, "error: could not write in file '%s'!\n", filename.c_str());
        return;
    }

    fout << data << endl;

    fout.close();
}
