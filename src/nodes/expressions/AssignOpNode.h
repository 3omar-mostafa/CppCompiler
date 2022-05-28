#ifndef ASSIGN_OP_NODE
#define ASSIGN_OP_NODE
#include "../Node.h"
#include "IdentifierNode.h"

class AssignOpNode : public ExpressionNode
{
    IdentifierNode *lhs;
    ExpressionNode *rhs;

public:
    AssignOpNode(yy::location loc, IdentifierNode *lhs, ExpressionNode *rhs) : ExpressionNode(loc)
    {
        this->lhs = lhs;
        this->rhs = rhs;
    }

    bool analyzeSemantic() override
    {
        // TODO::CHECK (bitwise AND instead of logical AND)
        if (!(rhs->analyzeSemantic() && lhs->analyzeSemantic()))
            return false;

        if (lhs->type == DTYPE_VOID || rhs->type == DTYPE_VOID)
            return false;
        // TODO::Check that lhs not literal (not rvalue)
        if (lhs->entryType == TYPE_CONST)
            return false;

        type = lhs->type;
        entryType = lhs->entryType; // check if constant
        return true;
    }

    string generateCode(CodeGenerationHelper *genHelper) override
    {
        string quad;
        quad = rhs->generateCode(genHelper);
        quad += Utils::convTypeToQuad(rhs->type, type);
        quad += Utils::opToQuad(OPR_POP, type) + " " + lhs->name + "\n";
        return quad;
    }
};
#endif