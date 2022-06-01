#ifndef NODE
#define NODE

#include <string>
#include <algorithm>
#include <vector>

#include "location.hpp"

#include "../utils/enums.h"
#include "../utils/utils.h"
#include "../symbolTable/SymbolTable.h"
#include "../helpers/CodeGenerationHelper.h"

class Node;
#include "../helpers/AnalysisHelper.h"

using namespace std;

class Node
{

public:
    yy::location loc;
    explicit Node(yy::location loc)
    {
        this->loc = loc;
    }
    virtual bool analyzeSemantic(AnalysisHelper *analysisHelper, bool used = false) = 0;
    virtual string generateCode(CodeGenerationHelper *genHelper) = 0;
};

class ExpressionNode : public Node
{
public:
    DataType type;       // int, float, ...
    EntryType entryType; // variable, const, function

    explicit ExpressionNode(yy::location loc) : Node(loc)
    {
    }
    bool analyzeSemantic(AnalysisHelper *analysisHelper, bool used = false) override = 0;
    string generateCode(CodeGenerationHelper *genHelper) override = 0;
};

typedef vector<Node *> StmtListNode;

#endif