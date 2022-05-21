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
    Node(yy::location loc)
    {
        this->loc = loc;
    }
    virtual bool analyzeSemantic() { return true; }
    virtual string generateCode();
};

class ExpressionNode : public Node
{
private:
    DataType type;       // int, float, ...
    EntryType entryType; // variable, const, function

public:
    ExpressionNode(yy::location loc) : Node(loc)
    {
    }
    virtual bool analyzeSemantic() { return true; }
    virtual string generateCode();
};

#endif