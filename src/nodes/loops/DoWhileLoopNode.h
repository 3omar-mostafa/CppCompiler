#ifndef DO_WHILE_LOOP_NODE
#define DO_WHILE_LOOP_NODE
#include "../Node.h"

class DoWhileLoopNode : public Node
{
    ExpressionNode *cond;
    Node *body;

public:
    DoWhileLoopNode(yy::location loc, ExpressionNode *cond, Node *body) : Node(loc)
    {
        this->cond = cond;
        this->body = body;
    }

    bool analyzeSemantic(bool used = false) override
    {
        auto scopeHelper = ScopeHelper::getInstance();

        if (scopeHelper->isInsideGlobalScope())
        {
            Utils::log("do-while loop is not allowed in global scope", loc, "error");
            return false;
        }

        bool check = true;
        scopeHelper->pushScope(SCOPE_LOOP, this);

        if (!(cond->analyzeSemantic(true) & body->analyzeSemantic()))
            check &= false;

        if (cond->type == DTYPE_VOID)
        {
            Utils::log("invalid conversion from '" + Utils::typeToQuad(cond->type) + "' to 'bool'", cond->loc, "error");
            check &= false;
        }

        scopeHelper->popScope();

        return check;
    }

    string generateCode(CodeGenerationHelper *genHelper) override
    {
        /*
        L1:
        ----------DO-BODY-------

        L2: (FOR CONTINUE)
        --------WHILE-COND------
        JMPNZ L2

        L3: (FOR BREAK)
        ----------EXIT----------

        */

        string quad;
        string l1 = genHelper->getNewLabel();
        string l2 = genHelper->getNewLabel();
        string l3 = genHelper->getNewLabel();

        quad += "L" + l1 + ":\n";

        genHelper->addContinueLabel(l2);
        genHelper->addBreakLabel(l3);

        quad += body->generateCode(genHelper);

        genHelper->removeContinueLabel();
        genHelper->removeBreakLabel();

        quad += "L" + l2 + ":\n";
        quad += cond->generateCode(genHelper);
        quad += Utils::opToQuad(OPR_JMPNZ, cond->type) + " L" + l1 + "\n";
        quad += "L" + l3 + ":\n";

        return quad;
    }

    ~DoWhileLoopNode() override
    {
        delete cond;
        delete body;

        cond = nullptr;
        body = nullptr;
    }
};
#endif