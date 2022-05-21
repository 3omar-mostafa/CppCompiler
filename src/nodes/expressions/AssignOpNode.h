#ifndef ASSIGN_OP_NODE
#define ASSIGN_OP_NODE
#include "../Node.h"

class AssignOpNode : public ExpressionNode
{
    ExpressionNode *lhs;
    ExpressionNode *rhs;

public:
    AssignOpNode(yy::location loc, ExpressionNode *lhs, ExpressionNode *rhs) : ExpressionNode(loc)
    {
        this->lhs = lhs;
        this->rhs = rhs;
    }

    virtual bool analyzeSemantic()
    {
        // TODO::CHECK (bitwise AND instead of logical AND)
        if (!(rhs->analyzeSemantic() && lhs->analyzeSemantic()))
            return false;

        if (lhs->type == DTYPE_VOID || lhs->type == DTYPE_VOID)
            return false;

        if (lhs->entryType == TYPE_CONST)
            return false;

        type = lhs->type;
        entryType = lhs->entryType; // check if constant
        return true;
    }

    // TODO:Implement
    virtual string generateCode()
    {
    }
};
#endif