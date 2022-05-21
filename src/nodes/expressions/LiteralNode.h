#ifndef LITERAL_NODE
#define LITERAL_NODE

#include "../Node.h"

class LiteralNode : public ExpressionNode
{
    string value;

public:
    LiteralNode(yy::location loc, const string& value) : ExpressionNode(loc)
    {
        this->value = value;
        this->entryType = EntryType::TYPE_CONST;
    }

    // TODO:Implement
    string generateCode() override
    {
    }
};

#endif