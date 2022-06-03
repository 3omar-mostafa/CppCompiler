#ifndef BLOCK_NODE
#define BLOCK_NODE

#include "../../utils/enums.h"
#include "../Node.h"
#include <iostream>

class StmtBlockNode : public Node
{
public:
    StmtListNode stmtList;

    explicit StmtBlockNode(yy::location loc) : Node(loc) {}

    StmtBlockNode(yy::location loc, const StmtListNode &stmtList) : Node(loc)
    {
        this->stmtList = stmtList;
    }

    bool analyzeSemantic(bool used = false) override
    {
        auto scopeHelper = ScopeHelper::getInstance();

         if (scopeHelper->isInsideGlobalScope())
         {
             Utils::log("Block is not allowed in global scope", loc, "error");
             return false;
         }

        bool check = true;

        scopeHelper->pushScope(SCOPE_BLOCK, this);

        for (auto& statement: stmtList)
        {
            check &= statement->analyzeSemantic();
        }

        scopeHelper->popScope();
        return check;
    }

    string generateCode() override
    {
        string quad;

        for (auto& statement: stmtList)
        {
            quad += statement->generateCode();
        }
        return quad;
    }

    ~StmtBlockNode() override
    {
        for (auto& statement: stmtList)
        {
            delete statement;
            statement = nullptr;
        }
    }
};

#endif
