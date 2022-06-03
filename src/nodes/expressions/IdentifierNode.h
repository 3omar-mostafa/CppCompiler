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

    bool analyzeSemantic(bool used)
    {
        auto scopeHelper = ScopeHelper::getInstance();

        EntryInfo* info = scopeHelper->lookup(name);
        if (!info)
        {
            Utils::log("'" + name + "' was not declared in this scope", loc, "error");
            return false;
        }
        type = info->type;
        entryType = info->entryType;

        if (used)
        {
            info->used += 1;
        }

        if (used && !info->initialized)
        {
            Utils::log("variable '" + name + "' used without being initialized", loc, "error");
            return false;
        }
        return true;
    }

    string generateCode() override
    {
        return Utils::opToQuad(OPR_PUSH, type) + " " + name + "\n";
    }
};
#endif