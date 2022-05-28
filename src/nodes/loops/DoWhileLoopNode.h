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

    bool analyzeSemantic() override
    {
        // TODO::add scopes

        if (!(cond->analyzeSemantic() && body->analyzeSemantic()))
            return false;

        if (cond->type == DTYPE_VOID)
            return false;

        return true;
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

        quad = "L" + l1 + ":\n";

        genHelper->addContinueLabel(l2);
        genHelper->addBreakLabel(l3);

        quad += body->generateCode(genHelper);

        genHelper->removeContinueLabel();
        genHelper->removeBreakLabel();

        quad = "L" + l2 + ":\n";
        quad += cond->generateCode(genHelper);
        quad += Utils::opToQuad(OPR_JMPNZ, cond->type) + " L" + l1 + "\n";
        quad = "L" + l3 + ":\n";

        return quad;
    }
};
#endif