#ifndef FUNCTION_CALL_NODE
#define FUNCTION_CALL_NODE

#include <iostream>
#include <vector>
#include <algorithm>

#include "../../utils/enums.h"
#include "../../utils/utils.h"
#include "../expressions/IdentifierNode.h"
#include "FunctionDeclarationNode.h"


typedef vector<ExpressionNode*> ExpressionList;

class FunctionCallNode : public ExpressionNode
{
    IdentifierNode* identifier;
    ExpressionList args;
    std::vector<DataType> functionParamsTypes;

public:
    FunctionCallNode(yy::location loc, IdentifierNode *identifier, vector<ExpressionNode *> args)
        : ExpressionNode(loc)
    {
        this->identifier = identifier;
        this->args = std::move(args);
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
            functionParamsTypes = info->paramsTypes;
        }

        for (auto& arg: args)
        {
            check &= arg->analyzeSemantic(analysisHelper);
        }

        if (args.size() != functionParamsTypes.size())
        {
            analysisHelper->log("wrong number of arguments", loc, "error");
            check &= false;
        }

        return check;
    }

    string generateCode(CodeGenerationHelper *genHelper) override
    {
        string quad;

        for (int i = (int) args.size() - 1; i >= 0; --i)
        {
            quad += args[i]->generateCode(genHelper);
            quad += Utils::convTypeToQuad(args[i]->type, functionParamsTypes[i]);
        }

        quad += "CALL " + identifier->name + " \n";
        return quad;
    }
};

#endif // FUNCTION_CALL_NODE
