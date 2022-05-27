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

    string generateCode() override
    {
        string quad;
        string l1 = "1";
        string l2 = "2";
        quad = varDec->generateCode();
        quad += "L" + l1 + ":\n";
        quad += cond->generateCode();
        quad += Utils::opToQuad(OPR_JMPZ, cond->type) + " L" + l2 + "\n";
        quad += body->generateCode();
        quad += step->generateCode();
        quad += Utils::opToQuad(OPR_JMP, cond->type) + " L" + l1 + "\n";
        quad += "L" + l2 + ":\n";

        return quad;
    }
};
#endif