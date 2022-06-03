#ifndef BREAK_NODE
#define BREAK_NODE
#include "../Node.h"

class BreakNode : public Node
{

public:
    explicit BreakNode(yy::location loc) : Node(loc) {}

    bool analyzeSemantic(bool used = false) override
    {
        auto scopeHelper = ScopeHelper::getInstance();

        if (!(scopeHelper->isInsideLoopScope() || scopeHelper->isInsideSwitchScope()))
        {
            Utils::log("break statement not within loop or switch", loc, "error");
            return false;
        }
        return true;
    }

    string generateCode(CodeGenerationHelper *genHelper) override
    {
        string quad;
        string l1 = genHelper->getBreakLabel();
        quad += Utils::opToQuad(OPR_JMP, DTYPE_INT) + " L" + l1 + "\n";

        return quad;
    }
};
#endif