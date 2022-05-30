#ifndef SWITCH_COND_NODE
#define SWITCH_COND_NODE
#include "../Node.h"
#include "CaseNode.h"

class SwitchCondNode : public Node
{
    ExpressionNode *cond;
    Node *body;

    vector<ExpressionNode *> caseLabels;
    vector<StmtListNode> caseStmts;

public:
    bool hasDefaultLabel;

    SwitchCondNode(yy::location loc, ExpressionNode *cond, Node *body) : Node(loc)
    {
        this->cond = cond;
        this->body = body;
        this->hasDefaultLabel = false;
    }

    virtual void populate()
    {
        StmtBlockNode *block = dynamic_cast<StmtBlockNode *>(body);

        if (block == NULL)
        {
            addCaseBlock(body);
            return;
        }

        for (int i = 0; i < block->stmtList.size(); ++i)
        {
            addCaseBlock(block->stmtList[i]);
        }
    }

    virtual void addCaseBlock(Node *stmt)
    {
        CaseNode *caseLabel;

        while (caseLabel = dynamic_cast<CaseNode *>(stmt))
        {
            caseLabels.push_back(caseLabel->cond);
            caseStmts.push_back(StmtListNode());

            stmt = caseLabel->body;
        }

        if (caseStmts.size() > 0)
        {
            caseStmts.back().push_back(stmt);
        }
    }

    bool analyzeSemantic(AnalysisHelper *analysisHelper, bool used = false) override
    {
        if (analysisHelper->isGlobalScope())
        {
            analysisHelper->log("switch statement is not allowed in global scope", loc, "error");
            return false;
        }

        populate();

        bool check = true;
        analysisHelper->pushScope(SCOPE_SWITCH, this);

        for (int i = 0; i < caseLabels.size(); ++i)
        {
            if (caseLabels[i] == NULL && hasDefaultLabel)
            {
                analysisHelper->log("multiple default labels in one switch", loc, "error");
                check &= false;
                break;
            }
            else if (caseLabels[i] == NULL)
                hasDefaultLabel = true;
        }

        if (!(cond->analyzeSemantic(analysisHelper) && body->analyzeSemantic(analysisHelper)))
            check &= false;

        if (cond->type == DTYPE_VOID)
        {
            analysisHelper->log("invalid conversion from '" + Utils::typeToQuad(cond->type) + "' to 'bool'", cond->loc, "error");
            check &= false;
        }

        analysisHelper->popScope();
        return check;
    }

    string generateCode(CodeGenerationHelper *genHelper) override
    {
        /*
        -------------COND------------
        JMPZ L1
        -------------BODY------------

        L1:
        ------COND-NOT-SATISFIED-----

        =============================
        --------------OR-------------
        =============================

        -------------COND------------
        JMPZ L1
        -------------BODY------------
        JMP L2

        L1: (ELSE)
        ------COND-NOT-SATISFIED-----

        L2:
        ------COND-SATISFIED-EXIT----

        */

        string quad;
        vector<pair<string, string>> labelPairs;
        string defaultLabel = "";
        string breakLabel = genHelper->getNewLabel();

        quad += cond->generateCode(genHelper);
        quad += Utils::opToQuad(OPR_POP, cond->type) + " SWITCH_COND_" + breakLabel + "\n";
        genHelper->addBreakLabel(breakLabel);

        for (int i = 0; i < caseLabels.size(); i++)
        {
            string l1 = genHelper->getNewLabel();
            if (caseLabels[i] == NULL)
            {
                defaultLabel = l1;
                labelPairs.push_back({"", l1});
            }
            else
            {
                string l2 = genHelper->getNewLabel();
                labelPairs.push_back({l1, l2});
            }
        }

        for (int i = 0; i < caseLabels.size(); i++)
        {
            if (caseLabels[i])
            {
                DataType resultType = max(cond->type, caseLabels[i]->type);

                quad += "L" + labelPairs[i].first + ":\n";
                quad += Utils::opToQuad(OPR_PUSH, cond->type) + " SWITCH_COND_" + genHelper->getBreakLabel() + "\n";
                quad += Utils::convTypeToQuad(cond->type, resultType);
                quad += caseLabels[i]->generateCode(genHelper);
                quad += Utils::convTypeToQuad(caseLabels[i]->type, resultType);
                quad += Utils::opToQuad(OPR_EQUAL, resultType) + "\n";
                quad += Utils::opToQuad(OPR_JMPZ, DTYPE_BOOL) + " L";

                if (i == caseLabels.size() - 1)
                {
                    quad += (hasDefaultLabel ? defaultLabel : breakLabel) + "\n";
                }
                else if (labelPairs[i + 1].first == "")
                {
                    quad += ((i + 1 == caseLabels.size() - 1) ? defaultLabel : labelPairs[i + 2].first) + "\n";
                }
                else
                { // my next is case
                    quad += labelPairs[i + 1].first + "\n";
                }
            }

            quad += "L" + labelPairs[i].second + ":\n";

            for (int j = 0; j < caseStmts[i].size(); j++)
            {
                quad += caseStmts[i][j]->generateCode(genHelper);
            }
        }

        genHelper->removeBreakLabel();
        quad += "L" + breakLabel + ":\n";

        return quad;
    }
};
#endif