#ifndef BREAK_NODE
#define BREAK_NODE
#include "../Node.h"

class BreakNode : public Node
{

public:
    BreakNode(yy::location loc) : Node(loc) {}

    bool analyzeSemantic(AnalysisHelper *analysisHelper) override
    {
        if (!analysisHelper->hasBreakScope())
        {
            analysisHelper->log("break statement not within loop or switch", loc, "error");
            return false;
        }
        return true;
    }

    string generateCode(CodeGenerationHelper *genHelper) override
    {
        // TODO:: SET LABEL TO THE NEAREST OUTER LABEL
        string quad;
        string l1 = genHelper->getBreakLabel();
        quad += Utils::opToQuad(OPR_JMP, DTYPE_INT) + " L" + l1 + "\n";

        return quad;
    }
};
#endif