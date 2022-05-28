#ifndef NODE
#define NODE

#include <string>
#include <algorithm>
#include <vector>

#include "location.hpp"

#include "../utils/enums.h"
#include "../utils/utils.h"
#include "../symbolTable/SymbolTable.h"
#include "../helpers/codeGenerationHelper.h"

using namespace std;

class Node
{
private:
    yy::location loc;

public:
    explicit Node(yy::location loc)
    {
        this->loc = loc;
    }
    virtual bool analyzeSemantic() { return true; }
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
    bool analyzeSemantic() override { return true; }
    string generateCode(CodeGenerationHelper *genHelper) override = 0;
};

typedef vector<Node *> StmtListNode;

#endif