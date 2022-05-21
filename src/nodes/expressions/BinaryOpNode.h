#ifndef BINARY_OP_NODE
#define BINARY_OP_NODE
#include "../Node.h"

class BianryOpNode : public ExpressionNode
{
    Operator op;
    ExpressionNode *lhs;
    ExpressionNode *rhs;

public:
    BianryOpNode(yy::location loc, ExpressionNode *lhs, Operator op, ExpressionNode *rhs) : ExpressionNode(loc)
    {
        this->lhs = lhs;
        this->op = op;
        this->rhs = rhs;
    }

    virtual bool analyzeSemantic()
    {
        if (!(lhs->analyzeSemantic() && rhs->analyzeSemantic()))
            return false;

        if (lhs->type == DTYPE_VOID || lhs->type == DTYPE_VOID)
            return false;

        if (Utils::isLogical(op))
            type = DTYPE_BOOL;
        else
            type = std::max(lhs->type, rhs->type);

        entryType = lhs->entryType && rhs->entryType; // check if both are constant
        return true;
    }

    // TODO:Implement
    virtual string generateCode()
    {
    }
};
#endif