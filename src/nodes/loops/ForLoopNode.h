#ifndef FOR_LOOP_NODE
#define FOR_LOOP_NODE
#include "../Node.h"
#include "../statements/VariableDeclarationNode.h"

class ForLoopNode : public Node
{
    VariableDeclarationNode *varDec;
    ExpressionNode *cond;
    ExpressionNode *step;
    Node *body;

public:
    ForLoopNode(yy::location loc, VariableDeclarationNode *varDec, ExpressionNode *cond, ExpressionNode *step, Node *body) : Node(loc)
    {
        this->varDec = varDec;
        this->cond = cond;
        this->step = step;
        this->body = body;
    }

    bool analyzeSemantic() override
    {
        // TODO::add scopes

        if (!(varDec->analyzeSemantic() && cond->analyzeSemantic() && step->analyzeSemantic() && body->analyzeSemantic()))
            return false;

        if (cond->type == DTYPE_VOID)
            return false;

        return true;
    }

    string generateCode(CodeGenerationHelper *genHelper) override
    {
        /*
        --------INIT-------
        L1:
        --------COND-------
        JMPZ L3
        --------BODY-------

        L2: (FOR CONTINUE)
        --------STEP-------
        JMP L1

        L3: (FOR BREAK)
        --------EXIT-------

        */

        string quad;
        string l1 = genHelper->getNewLabel();
        string l2 = genHelper->getNewLabel();
        string l3 = genHelper->getNewLabel();

        quad = varDec->generateCode(genHelper);
        quad += "L" + l1 + ":\n";
        quad += cond->generateCode(genHelper);
        quad += Utils::opToQuad(OPR_JMPZ, cond->type) + " L" + l3 + "\n";

        genHelper->addContinueLabel(l2);
        genHelper->addBreakLabel(l3);

        quad += body->generateCode(genHelper);

        genHelper->removeContinueLabel();
        genHelper->removeBreakLabel();

        quad += "L" + l2 + ":\n";
        quad += step->generateCode(genHelper);
        quad += Utils::opToQuad(OPR_JMP, cond->type) + " L" + l1 + "\n";
        quad += "L" + l3 + ":\n";

        return quad;
    }
};
#endif