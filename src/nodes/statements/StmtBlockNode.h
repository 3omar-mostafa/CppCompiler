#ifndef BLOCK_NODE
#define BLOCK_NODE

#include "../../utils/enums.h"
#include "../Node.h"
#include <iostream>

class StmtBlockNode : public Node
{
public:
    StmtListNode stmtList;

    StmtBlockNode(yy::location loc) : Node(loc) {}

    StmtBlockNode(yy::location loc, const StmtListNode &stmtList) : Node(loc)
    {
        this->stmtList = stmtList;
    }

    bool analyzeSemantic(AnalysisHelper *analysisHelper, bool used = false) override
    {
        // TODO::Uncomment this check after function impelementation
        // if (analysisHelper->isGlobalScope())
        // {
        //     analysisHelper->log("Block is not allowed in global scope", loc, "error");
        //     return false;
        // }

        bool check = true;

        analysisHelper->pushScope(SCOPE_BLOCK, this);

        for (int i = 0; i < stmtList.size(); ++i)
        {
            check &= stmtList[i]->analyzeSemantic(analysisHelper);
        }

        analysisHelper->popScope();
        return check;
    }

    string generateCode(CodeGenerationHelper *genHelper) override
    {
        string quad;

        for (int i = 0; i < stmtList.size(); ++i)
        {
            quad += stmtList[i]->generateCode(genHelper);
        }
        return quad;
    }
};

#endif
