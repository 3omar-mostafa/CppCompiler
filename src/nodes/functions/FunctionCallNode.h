#ifndef FUNCTION_CALL_NODE
#define FUNCTION_CALL_NODE

#include <iostream>
#include <vector>
#include <algorithm>

#include "../../utils/enums.h"
#include "../expressions/IdentifierNode.h"

typedef vector<ExpressionNode *> ExpressionList;
class FunctionCallNode : public ExpressionNode
{
    IdentifierNode *identifier;
    ExpressionList args;

public:
    FunctionCallNode(yy::location loc, IdentifierNode *identifier, vector<ExpressionNode *> args)
        : ExpressionNode(loc)
    {
        this->identifier = identifier;
        this->args = args;
    }

    bool analyzeSemantic(AnalysisHelper *analysisHelper, bool used = false) override
    {
        if (analysisHelper->isGlobalScope())
        {
            analysisHelper->log("function call is not allowed in global scope", loc, "error");
            return false;
        }

        bool check = true;

        EntryInfo *info;
        if (!analysisHelper->lookup(identifier->name, info))
        {
            analysisHelper->log("'" + identifier->name + "' was not declared in this scope", loc, "error");
            check &= false;
        }
        else
        {
            type = info->type;
            entryType = TYPE_CONST;
            info->used += 1;
        }

        for (int i = 0; i < args.size(); ++i)
        {
            check &= args[i]->analyzeSemantic(analysisHelper);
        }

        return check;
    }

    string generateCode(CodeGenerationHelper *genHelper) override
    {
        string quad;

        // std::reverse(args.begin(), args.end());

        for (auto &arg : args)
        {
            quad = arg->generateCode(genHelper) + quad;
        }

        quad += "CALL " + identifier->name + " \n";
        return quad;
    }
};

#endif // FUNCTION_CALL_NODE
