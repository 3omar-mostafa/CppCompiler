#ifndef RETURN_NODE
#define RETURN_NODE

#include <iostream>
#include <vector>
#include <algorithm>

#include "../../utils/enums.h"
#include "../Node.h"

class ReturnNode : public Node
{
    ExpressionNode *exp;
    FunctionDeclarationNode *function;

public:
    ReturnNode(yy::location loc, ExpressionNode* exp) : Node(loc)
    {
        this->exp = exp;
    }

    bool analyzeSemantic(bool used = false) override
    {
        auto scopeHelper = ScopeHelper::getInstance();

        if (scopeHelper->isInsideGlobalScope())
        {
            Utils::log("return is not allowed in global scope", loc, "error");
            return false;
        }

        bool check = true;

        function = scopeHelper->isInsideFunctionScope();
        if (!function)
        {
            Utils::log("return statement not within function", loc, "error");
            check &= false;
        }

        if (exp and function->type == DTYPE_VOID) {
            Utils::log("return statement can not have value in void functions", loc, "error");
            check &= false;
        }

        if (not exp and function->type != DTYPE_VOID) {
            Utils::log("return statement must have value in non-void functions", loc, "error");
            check &= false;
        }

        if (exp) {
            check &= exp->analyzeSemantic(true);
        }

        return check;
    }

    string generateCode(CodeGenerationHelper *genHelper) override
    {
        string quad;

        if (exp) {
            quad += exp->generateCode(genHelper);
            quad += Utils::convTypeToQuad(exp->type, function->type);
        }
        quad += "RET\n";
        return quad;
    }

    ~ReturnNode() override
    {
        delete exp;
        exp = nullptr;
    }
};

#endif // RETURN_NODE
