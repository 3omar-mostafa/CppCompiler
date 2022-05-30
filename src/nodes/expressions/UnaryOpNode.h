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

    bool analyzeSemantic(AnalysisHelper *analysisHelper, bool used = false) override
    {
        if (!(operand->analyzeSemantic(analysisHelper, true)))
            return false;

        if (operand->type == DTYPE_VOID)
        {
            analysisHelper->log("invalid operand of type '" + Utils::typeToQuad(operand->type), loc, "error");
            return false;
        }

        if (Utils::isLogical(op))
            type = DTYPE_BOOL;
        else
            type = operand->type;

        entryType = operand->entryType; // check if constant
        return true;
    }

    string generateCode(CodeGenerationHelper *genHelper) override
    {
        string quad;

        quad = operand->generateCode(genHelper);
        quad += Utils::convTypeToQuad(operand->type, type);

        quad += Utils::opToQuad(op, type) + "\n";
        return quad;
    }
};

#endif