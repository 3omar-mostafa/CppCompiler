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

    string generateCode() override
    {
        string quad;
        string l1 = "1";

        quad = "L" + l1 + ":\n";
        quad += body->generateCode();
        quad += cond->generateCode();
        quad += Utils::opToQuad(OPR_JMPNZ, cond->type) + " L" + l1 + "\n";

        return quad;
    }
};
#endif