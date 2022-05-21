#ifndef LITERAL_NODE
#define LITERAL_NODE
#include "../Node.h"

class LiteralNode : public ExpressionNode
{
    string value;

public:
    LiteralNode(yy::location loc, const string &vlaue) : ExpressionNode(loc)
    {
        this->value = value;
        this->isC
    }

    // TODO:Implement
    virtual string generateCode()
    {
    }
};
#endif