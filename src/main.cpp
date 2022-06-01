#include <iostream>
#include <fstream>
#include <parser.hpp>
#include "nodes/Node.h"

extern FILE *yyin;
extern std::string yyfilename;
extern Node *programRoot;

void writeToFile(const string& data, const string& filename);

int main(int argc, char *argv[])
{
    std::string outFile = "out.txt", symbolOutFile = "symbols.txt";

    switch (argc)
    {
    case 3:
        outFile = argv[2];
        break;
    case 4:
        outFile = argv[2];
        symbolOutFile = argv[3];
        break;
    }

    if (argc >= 2)
    {
        yyin = fopen(argv[1], "r");
        if (yyin == nullptr)
        {
            fprintf(stderr, "error: could not open the input file %s", argv[1]);
            exit(1);
        }
        yyfilename = argv[1];
        std::cout << "Compiling " << argv[1] << std::endl;
    }
    else
    {
        std::cerr << "No files were given, reading input from stdin" << std::endl;
    }

    yy::parser parser;
    parser.parse();

    AnalysisHelper *analysisHelper = new AnalysisHelper(argv[1]);

    std::cout << "Start Analyzing \n";
    bool analyze = programRoot->analyzeSemantic(analysisHelper);
    if (!analyze)
    {
        std::cerr << "Semantic Error Can not generate quadruples\n";
        delete analysisHelper;
        exit(1);
    }
    std::cout << "Finished Semantic analysis\n";
    CodeGenerationHelper *genHelper = new CodeGenerationHelper();

    std::string quad = programRoot->generateCode(genHelper);

    std::cout << "Finished code generation\n";

    writeToFile(quad, outFile);
    writeToFile(analysisHelper->getSymbolTableString(), symbolOutFile);
    writeToFile(analysisHelper->getSymbolTableRepresentation(), "symbols2.txt");

    delete analysisHelper;
    delete genHelper;
    return 0;
}

void writeToFile(const string& data, const string& filename)
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
