#ifndef LITERAL_NODE
#define LITERAL_NODE

#include "../Node.h"

class LiteralNode : public ExpressionNode
{
    string value;

public:
    LiteralNode(yy::location loc, const string &value, DataType type) : ExpressionNode(loc)
    {
        this->type = type;
        this->value = value;
        this->entryType = EntryType::TYPE_CONST;
    }

    bool analyzeSemantic(bool used = false) override
    {
        return true;
    }

    string generateCode() override
    {
        return Utils::opToQuad(OPR_PUSH, type) + " " + value + "\n";
    }
};

#endif