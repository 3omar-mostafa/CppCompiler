#ifndef IDENTIFIER_NODE
#define IDENTIFIER_NODE
#include "../Node.h"

class IdentifierNode : public ExpressionNode
{
public:
    string name;
    IdentifierNode(yy::location loc, const string &name) : ExpressionNode(loc)
    {
        this->name = name;
    }

    bool analyzeSemantic(AnalysisHelper *analysisHelper) override
    {
        EntryInfo info;
        if (!analysisHelper->lookup(name, info))
        {
            analysisHelper->log("'" + name + "' was not declared in this scope", loc, "error");
            return false;
        }
        type = info.type;
        entryType = info.entryType;
        return true;
    }

    string generateCode(CodeGenerationHelper *genHelper) override
    {
        return Utils::opToQuad(OPR_PUSH, type) + " " + name + "\n";
    }
};
#endif