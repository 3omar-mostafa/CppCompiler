#ifndef WHILE_LOOP_NODE
#define WHILE_LOOP_NODE
#include "../Node.h"

class WhileLoopNode : public Node
{
    ExpressionNode *cond;
    Node *body;

public:
    WhileLoopNode(yy::location loc, ExpressionNode *cond, Node *body) : Node(loc)
    {
        this->cond = cond;
        this->body = body;
    }

    bool analyzeSemantic(bool used = false) override
    {
        auto scopeHelper = ScopeHelper::getInstance();

        if (scopeHelper->isInsideGlobalScope())
        {
            Utils::log("while loop is not allowed in global scope", loc, "error");
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

        return true;
    }

    string generateCode() override
    {
        /*
        L1: (FOR CONTINUE)
        --------WHILE-COND------

        --------WHILE-BODY------

        L2: (FOR BREAK)
        ----------EXIT----------

        */

        auto genHelper = CodeGenerationHelper::getInstance();

        string quad;
        string l1 = genHelper->getNewLabel();
        string l2 = genHelper->getNewLabel();

        quad = l1 + ":\n";
        quad += cond->generateCode();
        quad += Utils::opToQuad(OPR_JMPZ, cond->type) + " " + l2 + "\n";

        genHelper->addContinueLabel(l1);
        genHelper->addBreakLabel(l2);

        quad += body->generateCode();

        genHelper->removeContinueLabel();
        genHelper->removeBreakLabel();

        quad += Utils::opToQuad(OPR_JMP, cond->type) + " " + l1 + "\n";
        quad += l2 + ":\n";

        return quad;
    }

    ~WhileLoopNode() override
    {
        delete cond;
        delete body;

        cond = nullptr;
        body = nullptr;
    }
};
#endif