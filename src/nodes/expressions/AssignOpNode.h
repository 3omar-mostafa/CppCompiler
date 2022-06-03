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

    bool analyzeSemantic(bool used = false) override
    {
        auto scopeHelper = ScopeHelper::getInstance();

        if (scopeHelper->isInsideGlobalScope())
        {
            Utils::log("assign statement is not allowed in global scope", loc, "error");
            return false;
        }

        if (!(rhs->analyzeSemantic(true) & lhs->analyzeSemantic(false)))
            return false;

        if (lhs->type == DTYPE_VOID || rhs->type == DTYPE_VOID)
        {
            Utils::log("invalid conversion from '" + Utils::typeToQuad(rhs->type) + "' to '" + Utils::typeToQuad(lhs->type) + "'", rhs->loc, "error");
            return false;
        }

        if (lhs->entryType == TYPE_CONST)
        {
            Utils::log("assignment of read-only (const) variable '" + lhs->name + "'", lhs->loc, "error");
            return false;
        }

        EntryInfo *info = scopeHelper->lookup(lhs->name);
        info->initialized = true;

        type = lhs->type;
        entryType = lhs->entryType;
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

    ~AssignOpNode() override
    {
        delete lhs;
        lhs = nullptr;
        delete rhs;
        rhs = nullptr;
    }
};
#endif