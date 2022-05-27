#ifndef BINARY_OP_NODE
#define BINARY_OP_NODE

#include "../Node.h"

class BinaryOpNode : public ExpressionNode
{
    Operator op;
    ExpressionNode *lhs;
    ExpressionNode *rhs;

public:
    BinaryOpNode(yy::location loc, ExpressionNode *lhs, Operator op, ExpressionNode *rhs) : ExpressionNode(loc)
    {
        this->lhs = lhs;
        this->op = op;
        this->rhs = rhs;
    }

    bool analyzeSemantic() override
    {
        if (!(lhs->analyzeSemantic() && rhs->analyzeSemantic()))
            return false;

        if (lhs->type == DTYPE_VOID || rhs->type == DTYPE_VOID)
            return false;

        if (Utils::isLogical(op))
            type = DTYPE_BOOL;
        else
            type = std::max(lhs->type, rhs->type);

        entryType = (lhs->entryType == EntryType::TYPE_CONST) && (rhs->entryType == EntryType::TYPE_CONST)
                        ? EntryType::TYPE_CONST
                        : EntryType::TYPE_VAR;
        return true;
    }

    string generateCode() override
    {
        /*
        PUSH DATATYPE LHS
        PUSH DATATYPE RHS
        OPERATION
        */
        string quad;
        DataType op_type = std::max(lhs->type, rhs->type);

        quad = lhs->generateCode();
        quad += Utils::convTypeToQuad(lhs->type, op_type);

        quad += rhs->generateCode();
        quad += Utils::convTypeToQuad(rhs->type, op_type);

        quad += Utils::opToQuad(op, type) + "\n";

        return quad;
    }
};

#endif