#ifndef UNARY_OP_NODE
#define UNARY_OP_NODE

#include "nodes/Node.h"

class UnaryOpNode : public ExpressionNode
{
    Operator op;
    ExpressionNode *operand;

public:
    UnaryOpNode(yy::location loc, Operator op, ExpressionNode *operand) : ExpressionNode(loc)
    {
        this->op = op;
        this->operand = operand;
    }

    bool analyzeSemantic() override
    {
        if (!(operand->analyzeSemantic()))
            return false;

        if (operand->type == DTYPE_VOID)
            return false;

        if (Utils::isLogical(op))
            type = DTYPE_BOOL;
        else
            type = operand->type;

        entryType = operand->entryType; // check if constant
        return true;
    }

    string generateCode() override
    {
        string quad;

        quad = operand->generateCode();
        quad += Utils::convTypeToQuad(operand->type, type);

        quad += Utils::opToQuad(op, type) + "\n";
        return quad;
    }
};

#endif