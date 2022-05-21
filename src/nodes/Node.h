#ifndef NODE
#define NODE

#include <location.hpp>
#include <string>
#include "../utils/enums.h"
#include "../utils/utils.h"
#include "../symbolTable/SymbolTable.h"
#include <algorithm>

using std::string;

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
    virtual string generateCode() = 0;
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
    string generateCode() override = 0;
};

#endif