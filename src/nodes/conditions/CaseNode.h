#ifndef CASE_NODE
#define CASE_NODE
#include "../Node.h"

class CaseNode : public Node
{

public:
    ExpressionNode *cond;
    Node *body;
    CaseNode(yy::location loc, ExpressionNode *cond, Node *body) : Node(loc)
    {
        this->cond = cond;
        this->body = body;
    }

    bool analyzeSemantic(AnalysisHelper *analysisHelper, bool used = false) override
    {
        if (!analysisHelper->hasSwitchScope())
        {
            analysisHelper->log("case label not within switch statement", loc, "error");
            return false;
        }

        bool check = true;

        if (cond)
        { // case label
            check = cond->analyzeSemantic(analysisHelper, true);

            if (check && cond->entryType != TYPE_CONST)
            {
                analysisHelper->log("constant expression required in case label", cond->loc, "error");
                check &= false;
            }
            if (check && !Utils::isIntegerType(cond->type))
            {
                analysisHelper->log("case quantity not an integer", cond->loc, "error");
                check &= false;
            }
        }

        check &= body->analyzeSemantic(analysisHelper);

        return check;
    }

    string generateCode(CodeGenerationHelper *genHelper) override
    {
        return "";
    }

    ~CaseNode() override
    {
        delete cond;
        cond = nullptr;
        delete body;
        body = nullptr;
    }
};
#endif