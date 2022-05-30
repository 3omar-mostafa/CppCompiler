#ifndef CONTINUE_NODE
#define CONTINUE_NODE
#include "../Node.h"

class ContinueNode : public Node
{

public:
    ContinueNode(yy::location loc) : Node(loc) {}

    bool analyzeSemantic(AnalysisHelper *analysisHelper, bool used = false) override
    {
        if (!analysisHelper->hasLoopScope())
        {
            analysisHelper->log("continue statement not within loop", loc, "error");
            return false;
        }
        return true;
    }

    string generateCode(CodeGenerationHelper *genHelper) override
    {
        // TODO:: SET LABEL TO THE NEAREST LOOP LABEL
        string quad;
        string l1 = genHelper->getContinueLabel();
        quad += Utils::opToQuad(OPR_JMP, DTYPE_INT) + " L" + l1 + "\n";

        return quad;
    }
};
#endif