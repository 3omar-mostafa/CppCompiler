#ifndef CONTINUE_NODE
#define CONTINUE_NODE
#include "../Node.h"

class ContinueNode : public Node
{

public:
    explicit ContinueNode(yy::location loc) : Node(loc) {}

    bool analyzeSemantic(bool used = false) override
    {
        auto scopeHelper = ScopeHelper::getInstance();

        if (!scopeHelper->isInsideLoopScope())
        {
            Utils::log("continue statement not within loop", loc, "error");
            return false;
        }
        return true;
    }

    string generateCode() override
    {
        auto genHelper = CodeGenerationHelper::getInstance();

        string quad;
        string l1 = genHelper->getContinueLabel();
        quad += Utils::opToQuad(OPR_JMP, DTYPE_INT) + " " + l1 + "\n";

        return quad;
    }
};
#endif