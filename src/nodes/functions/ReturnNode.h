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

public:
    ReturnNode(yy::location loc, ExpressionNode *exp)
        : Node(loc)
    {
        this->exp = exp;
    }

    bool analyzeSemantic(AnalysisHelper *analysisHelper, bool used = false) override
    {
        if (analysisHelper->isGlobalScope())
        {
            analysisHelper->log("return is not allowed in global scope", loc, "error");
            return false;
        }

        bool check = true;

        if (!analysisHelper->hasFunctionScope())
        {
            analysisHelper->log("return statement not within function", loc, "error");
            check &= false;
        }

        if (exp)
            check &= exp->analyzeSemantic(analysisHelper, true);

        return check;
    }

    string generateCode(CodeGenerationHelper *genHelper) override
    {
        string quad;

        if (exp)
            quad += exp->generateCode(genHelper);
        quad += "RET\n";
        return quad;
    }
};

#endif // FUNCTION_CALL_NODE
