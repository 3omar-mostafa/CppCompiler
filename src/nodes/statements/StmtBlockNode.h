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

    bool analyzeSemantic() override
    {
        // TODO:: Check scopes

        bool check = true;

        for (int i = 0; i < stmtList.size(); ++i)
        {
            check = check & stmtList[i]->analyzeSemantic();
        }

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
