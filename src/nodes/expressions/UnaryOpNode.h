#ifndef UNARY_OP_NODE
#define UNARY_OP_NODE

#include "../Node.h"

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

    bool analyzeSemantic(bool used = false) override
    {
        auto scopeHelper = ScopeHelper::getInstance();

        if (!(operand->analyzeSemantic(true)))
            return false;

        if (operand->type == DTYPE_VOID)
        {
            Utils::log("invalid operand of type '" + Utils::typeToQuad(operand->type), loc, "error");
            return false;
        }

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

    ~UnaryOpNode() override
    {
        delete operand;
        operand = nullptr;
    }
};

#endif